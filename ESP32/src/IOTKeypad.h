#ifndef IOT_KEYPAD_H
#define IOT_KEYPAD_H

#include <consts.h>
#include <cstdint>
#include <I2CKeyPad.h>

#include <IOTFiles.h>

class IOTKeypad {
public:
    IOTKeypad() : keypad(KEYPAD_ADDR), key_still_pressed(false) {};
    IOTKeypad(const IOTKeypad&) = default;
    IOTKeypad& operator=(const IOTKeypad&) = delete;

    void init();
    char tick();

private:
    I2CKeyPad keypad;
    bool key_still_pressed;
};

#endif
