#ifndef IOT_CONSTS_H
#define IOT_CONSTS_H

#include <cstdint>
#include <map>
#include <vector>

#include <WString.h>
#include <Adafruit_SSD1306.h>

struct User {
    const String id;
    const String uid;
};

using UserList = std::vector<User>;

enum FileName {
    ATTENDANCE_LOG,
    USER_LIST,
    PENDING_USER_LIST,
    AMOUNT_OF_FILES,
};


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
const uint16_t SHORT_MSG_DELAY = 1000;
const uint16_t DISPLAY_BLINK_DELAY = 600;
const uint16_t MILLIS_TIMEOUT = 1000 * 30;

// GoogleSheet consts
const String GOOGLE_SCRIPT_ID = "AKfycbyNgiYIkcQo-valD85SHRbkjEFaytGMBlk8dUfbdm2cUYEnLMSYKyyBRF7OxZy4moTh";
const std::map<String, String> SSID_TO_PASS = {{"Vladimir Computin", "100200300"}};
const String ADD_USER = "add_user";
const String ADD_LOG = "add_log";
const String GET_USERS = "get_approved_users";

// IOTKeypad consts
const uint8_t KEYPAD_ADDR = 0x20;
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

// Messages
const String SWIPE_CARD = "Please \nswipe card";
const String ID_NOT_VALID = "ID is not valid";
const String SENDING_USER_INFO = "Sending \nrequest to\napproval";
const String ID_TAKEN = "ID \nregistered\nto another\ncard";
const String TIMEOUT = "Timeout \npassed";
const String WAITING_FOR_APPROVAL = "Card is\npending \napproval";
const String REGISTER_MENU = "Press Key:\n A:sign up\n C:cancel";
const String ENTER_ID = "Enter a 9\ndigit ID.\nPress B to\nundo";
const String INVALID_KEY = "Wrong key";

const uint16_t STACK_SIZE = 16384;
const uint16_t JSON_SIZE = 1024;

#endif
