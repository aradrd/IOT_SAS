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

    void init() {
        if (keypad.begin() == false)
        {
            Serial.println("Can't communicate to keypad.");
        }
        else
        {
            keypad.loadKeyMap(keymap);
        }
    }

    char tick() {
        char key = '\0';
        Wire.requestFrom(KEYPAD_ADDR, (uint8_t)1);
        if (!key_still_pressed && keypad.isPressed())
        {
            if (keypad.getKey() == I2C_KEYPAD_FAIL){
                return key;
            }
            key = keypad.getLastChar(); // note we want the translated char    
            //Serial.print(key);
            //Serial.println(" pressed.");
        }
        key_still_pressed = keypad.isPressed();
        return key;
    }

private:
    I2CKeyPad keypad;
    bool key_still_pressed;
};

#endif