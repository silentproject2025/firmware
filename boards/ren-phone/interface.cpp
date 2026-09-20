/*
 * Ren Phone - DIY ESP32-S3 handheld
 * Board interface for Bruce firmware
 *
 * Hardware:
 *   - ESP32-S3 (16MB flash, OPI PSRAM)
 *   - ILI9341 240x320 TFT on SPI (TFT_eSPI, pins in pins_arduino.h)
 *   - XPT2046 resistive touch on its own pins (read by CYD28_TouchR, bit-banged)
 *   - microSD in SDIO 1-bit mode (SD_MMC): CLK=39 CMD=38 D0=40
 *   - INMP441 mic, WS2812 LED (GPIO48), vibration motor (GPIO18), battery ADC (GPIO8)
 */

#include "CYD28_TouchscreenR.h"
#include "core/bus_HAL.h"
#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>
#include <globals.h>
#include <interface.h>

// SD card SDIO pins (1-bit mode)
#define REN_SD_CLK 39
#define REN_SD_CMD 38
#define REN_SD_D0 40

// Backlight PWM (attached after the TFT is initialised)
#define REN_BL_FREQ 5000
#define REN_BL_BITS 8

// Optional touch axis inversion, set from ren-phone.ini
#ifndef REN_TOUCH_INVERT_X
#define REN_TOUCH_INVERT_X 0
#endif
#ifndef REN_TOUCH_INVERT_Y
#define REN_TOUCH_INVERT_Y 0
#endif

// XPT2046 is read in software SPI (same approach as the original CYD boards):
// it keeps the touch chip independent from the hardware SPI controllers that
// are shared between the display and the external radio modules.
CYD28_TouchR touch(320, 240);

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // SD card via SDIO (SD_MMC wrapper in lib/HAL)
    SD.setPins(REN_SD_CLK, REN_SD_CMD, REN_SD_D0);

    // Vibration motor: keep it off (Bruce has no driver for it)
    pinMode(REN_VIB_PIN, OUTPUT);
    digitalWrite(REN_VIB_PIN, LOW);

    // Touch chip
    pinMode(XPT2046_SPI_CONFIG_CS_GPIO_NUM, OUTPUT);
    digitalWrite(XPT2046_SPI_CONFIG_CS_GPIO_NUM, HIGH);
    if (!touch.begin()) { Serial.println("Touchscreen initialization failed!"); }

    // Panel is a standard (non-inverted) ILI9341
    bruceConfig.colorInverted = 0;
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup (after the TFT is initialised)
***************************************************************************************/
void _post_setup_gpio() {
    // Brightness control must be initialised after the TFT
    pinMode(TFT_BL, OUTPUT);
    ledcAttach(TFT_BL, REN_BL_FREQ, REN_BL_BITS);
    ledcWrite(TFT_BL, 255);
}

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value (0-100)
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    int dutyCycle;
    if (brightval == 100) dutyCycle = 255;
    else if (brightval == 75) dutyCycle = 130;
    else if (brightval == 50) dutyCycle = 70;
    else if (brightval == 25) dutyCycle = 20;
    else if (brightval == 0) dutyCycle = 0;
    else dutyCycle = ((brightval * 255) / 100);

    ledcWrite(TFT_BL, dutyCycle);
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static long d_tmp = 0;
    if (millis() - d_tmp > 200 || LongPress) {
        // nested on purpose: don't poll the touch chip more often than needed
        if (touch.touched()) {
            auto t = touch.getPointScaled();

            // Map raw (landscape) coordinates to the current TFT rotation
            if (bruceConfigPins.rotation == 3) {
                t.y = (tftHeight + 20) - t.y;
                t.x = tftWidth - t.x;
            }
            if (bruceConfigPins.rotation == 0) {
                int tmp = t.x;
                t.x = tftWidth - t.y;
                t.y = tmp;
            }
            if (bruceConfigPins.rotation == 2) {
                int tmp = t.x;
                t.x = t.y;
                t.y = (tftHeight + 20) - tmp;
            }
            // rotation == 1 (landscape, default): no transform

#if REN_TOUCH_INVERT_X
            t.x = tftWidth - t.x;
#endif
#if REN_TOUCH_INVERT_Y
            t.y = (tftHeight + 20) - t.y;
#endif

            if (!wakeUpScreen()) AnyKeyPress = true;
            else goto END;

            // Touch point global variable
            touchPoint.x = t.x;
            touchPoint.y = t.y;
            touchPoint.pressed = true;
            touchHeatMap(touchPoint);
        END:
            d_tmp = millis();
        }
    }
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (deep sleep, wakes on the BOOT button)
**********************************************************************/
void powerOff() {
    ledcWrite(TFT_BL, 0);
    tft.writecommand(0x10); // ILI9341 SLPIN
    digitalWrite(REN_VIB_PIN, LOW);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turn off the device (name is odd btw)
**********************************************************************/
void checkReboot() {}
