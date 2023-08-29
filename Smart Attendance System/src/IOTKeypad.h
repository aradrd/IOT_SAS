#ifndef IOT_KEYPAD_H
#define IOT_KEYPAD_H

#include <consts.h>
#include <cstdint>
#include <I2CKeyPad.h>

class IOTKeypad {
public:
    IOTKeypad(Display& display) : keypad(KEYPAD_ADDR), display(display), key_still_pressed(false) {};
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

    void tick() {
        Wire.requestFrom(KEYPAD_ADDR, (uint8_t)1);
        if (!key_still_pressed && keypad.isPressed())
        {
            char ch = keypad.getChar(); // note we want the translated char
            Serial.print(ch);
            Serial.println(" pressed.");
            display.print(ch);
        }
        key_still_pressed = keypad.isPressed();
    }

private:
    I2CKeyPad keypad;
    Display& display;
    bool key_still_pressed;
};

#endif