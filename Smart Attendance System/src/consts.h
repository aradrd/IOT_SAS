#ifndef IOT_CONSTS_H
#define IOT_CONSTS_H

#include <cstdint>
#include <map>
#include <WString.h>
#include <Adafruit_SSD1306.h>

// main consts
const uint16_t BAUD = 9600;
const uint16_t I2C_CLOCK = 40000;

const uint8_t DISPLAY1_WIDTH = 128;
const uint8_t DISPLAY1_HEIGHT = 64;

const uint8_t DISPLAY2_WIDTH = 128;
const uint8_t DISPLAY2_HEIGHT = 32;

const uint8_t I2C_SDA_2 = 33;
const uint8_t I2C_SCL_2 = 32;

const uint8_t DELAY = 10;

// GoogleSheet consts
const String GOOGLE_SCRIPT_ID = "AKfycbxzjMSDoVmp_b9QqPIoUlQNiA9kKDV0cGMx_1QTSBuceherA8vr9LnqaSnHrYxRU3tv";
const std::map<String, String> SSID_TO_PASS = {{"TechPublic", ""}};

// IOTKeypad consts
const uint8_t KEYPAD_ADDR = 0x20;
char keymap[17] = "147*2580369#ABCD";

// Display consts
const uint8_t DISPLAY_RST = -1; // -1 means shared (whatever that means).
const uint8_t DISPLAY_ADDR = 0x3C;
const uint8_t DISPLAY_TEXT_SIZE = 1;
const uint8_t DISPLAY_TEXT_COLOR = WHITE;

// RFID consts
const uint8_t RFID_RST = 2;
const uint8_t RFID_SS = 5;

#endif