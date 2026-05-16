#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

#define USB_VID 0x303a
#define USB_PID 0x1001

static const uint8_t TX = 43;
static const uint8_t RX = 44;

static const uint8_t SDA = 13;
static const uint8_t SCL = 15;

static const uint8_t SS = 21;
static const uint8_t MOSI = 45;
static const uint8_t MISO = 42;
static const uint8_t SCK = 47;

/* TFT definitions */
#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT 160
#define USER_SETUP_LOADED 1

#define ILI9341_DRIVER 1
#define TFT_HEIGHT 320
#define TFT_WIDTH 240

#define TFT_BACKLIGHT_ON 1
#define TFT_BL -1
#define TFT_RST 1
#define TFT_DC 14
#define TFT_MISO 42
#define TFT_MOSI 45
#define TFT_SCLK 47
#define TFT_CS 21

#define SMOOTH_FONT 1
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000

/* Buttons */
#define HAS_BTN 1
#define SEL_BTN 0
#define UP_BTN 41
#define ESC_BTN 3
#define DW_BTN 46
#define BTN_ACT LOW
#define BTN_ALIAS "Ok"

/* Communication Buses */
#define SPI_SCK_PIN 47
#define SPI_MOSI_PIN 45
#define SPI_MISO_PIN 42
#define SPI_SS_PIN 21

#define GROVE_SDA 13
#define GROVE_SCL 15

#endif /* Pins_Arduino_h */
