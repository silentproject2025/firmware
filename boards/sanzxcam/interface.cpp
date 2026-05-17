#include "core/powerSave.h"
#include "core/utils.h"
#include <globals.h>
#include <interface.h>
#include <Button.h>

volatile bool nxtPress = false;
volatile bool prvPress = false;
volatile bool ecPress = false;
volatile bool slPress = false;

static void onButtonSingleClickCbUp(void *button_handle, void *usr_data) { nxtPress = true; }
static void onButtonSingleClickCbDw(void *button_handle, void *usr_data) { prvPress = true; }
static void onButtonSingleClickCbEsc(void *button_handle, void *usr_data) { ecPress = true; }

Button *btnUp;
Button *btnDw;
Button *btnEsc;

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // setup buttons
    button_config_t btUp = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 600,
        .short_press_time = 120,
        .gpio_button_config = {
            .gpio_num = UP_BTN,
            .active_level = 0,
        },
    };
    button_config_t btDw = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 600,
        .short_press_time = 120,
        .gpio_button_config = {
            .gpio_num = DW_BTN,
            .active_level = 0,
        },
    };
    button_config_t btEsc = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 600,
        .short_press_time = 120,
        .gpio_button_config = {
            .gpio_num = ESC_BTN,
            .active_level = 0,
        },
    };

    pinMode(SEL_BTN, INPUT_PULLUP);

    btnUp = new Button(btUp);
    btnUp->attachSingleClickEventCb(&onButtonSingleClickCbUp, NULL);

    btnDw = new Button(btDw);
    btnDw->attachSingleClickEventCb(&onButtonSingleClickCbDw, NULL);

    btnEsc = new Button(btEsc);
    btnEsc->attachSingleClickEventCb(&onButtonSingleClickCbEsc, NULL);

    // setup backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);
}

/*********************************************************************
**  Function: setBrightness
**  set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, 0);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static long tm = 0;
    static bool btn_pressed = false;
    bool selPressed = false;

    if (nxtPress || prvPress || ecPress || slPress || selPressed) btn_pressed = true;

    if (millis() - tm > 200 || LongPress) {
        if (digitalRead(SEL_BTN) == BTN_ACT) {
            selPressed = true;
            btn_pressed = true;
        }

        if (btn_pressed) {
            btn_pressed = false;
            if (!wakeUpScreen()) AnyKeyPress = true;
            else return;

            SelPress = slPress || selPressed;
            EscPress = ecPress;
            NextPress = nxtPress;
            PrevPress = prvPress;

            nxtPress = false;
            prvPress = false;
            ecPress = false;
            slPress = false;
        }
    }
}

void powerOff() {
    tft.fillScreen(bruceConfig.bgColor);
    tft.writecommand(0x10);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)SEL_BTN, BTN_ACT);
    esp_deep_sleep_start();
}

void checkReboot() {
    // Basic reboot check can be implemented here if needed
}
