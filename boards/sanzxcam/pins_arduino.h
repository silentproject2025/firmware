#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#ifndef DEVICE_NAME
#define DEVICE_NAME "Sanzxcam"
#endif

// Main SPI Bus
#define SPI_SS_PIN 21
#define SPI_MOSI_PIN 45
#define SPI_MISO_PIN -1
#define SPI_SCK_PIN 47

static const uint8_t SS = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t MISO = SPI_MISO_PIN;
static const uint8_t SCK = SPI_SCK_PIN;

// Main I2C Bus
#define GROVE_SDA 13
#define GROVE_SCL 15
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// BadUSB
#define BAD_TX GROVE_SDA
#define BAD_RX GROVE_SCL
#define USB_as_HID 1

// TFT_eSPI display
#define USER_SETUP_LOADED 1
#define ILI9341_DRIVER 1
#define USE_TFT_ESPI 1
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_MOSI SPI_MOSI_PIN
#define TFT_MISO -1
#define TFT_SCLK SPI_SCK_PIN
#define TFT_CS SPI_SS_PIN
#define TFT_DC 14
#define TFT_RST 1
#define TFT_BL 38 // Not specified

#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000

// Display Setup
#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT 160

// Font Sizes
#define FP 1
#define FM 2
#define FG 3

// Buttons & Navigation
#define BTN_ALIAS "\"OK\""
#define HAS_3_BUTTONS
#define SEL_BTN 0
#define UP_BTN 2
#define DW_BTN 3
#define ESC_BTN 46
#define BTN_ACT LOW

// Default IR, RF and RFID Configs
#define TXLED -1
#define RXLED -1
#define LED_ON HIGH
#define LED_OFF LOW

#endif /* Pins_Arduino_h */
