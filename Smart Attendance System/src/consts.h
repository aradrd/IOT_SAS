#ifndef IOT_CONSTS_H
#define IOT_CONSTS_H

#include <cstdint>
#include <map>
#include <WString.h>
#include <Adafruit_SSD1306.h>

// main consts
const uint16_t BAUD = 9600;
const uint16_t I2C_CLOCK = 40000;

const uint8_t DISPLAY_WIDTH = 128;
const uint8_t DISPLAY_HEIGHT = 64;

const uint8_t I2C_SDA_2 = 33;
const uint8_t I2C_SCL_2 = 32;

// Time consts
const uint8_t DELAY = 10;
const uint16_t MSG_DELAY = 5000;
const long MICROS_TIMEOUT = 1000000 * 5;

// GoogleSheet consts
const String GOOGLE_SCRIPT_ID = "AKfycbzqxFAFERQau9DFvuIWVHVk8ySXi_X75JlsON0nyMEZaiRpd0id7Kj9zkBRLrZ4XbyF";
const std::map<String, String> SSID_TO_PASS = {{"TechPublic", ""}};

// IOTKeypad consts
const uint8_t KEYPAD_ADDR = 0x20;
char keymap[17] = "147*2580369#ABCD";
const char BACKSPACE_KEY = 'B';
const char OK_KEY = 'A';
const char CANCEL_KEY = 'C';

const uint8_t ID_LENGTH = 9;

// Display consts
const uint8_t DISPLAY_RST = -1; // -1 means shared (whatever that means).
const uint8_t DISPLAY_ADDR = 0x3C;
const uint8_t DISPLAY_TEXT_SIZE = 2;
const uint8_t DISPLAY_TEXT_COLOR = WHITE;

// RFID consts
const uint8_t RFID_RST = 2;
const uint8_t RFID_SS = 5;

// IOTFS consts
const String USERLIST_PATH = "/user_list.csv";
const String ATTENDANCELOG_PATH = "/attendance_log.csv";
const String PENDINGUSERLIST_PATH = "/pending_user_list.csv";

#endif