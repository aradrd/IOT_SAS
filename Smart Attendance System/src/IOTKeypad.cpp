#include <IOTKeypad.h>
#include <consts.h>

char keymap[17] = "147*2580369#ABCD";

void IOTKeypad::init() {
    if (keypad.begin() == false)
    {
        Serial.println("Can't communicate to keypad.");
    }
    else
    {
        keypad.loadKeyMap(keymap);
    }
}

char IOTKeypad::tick() {
    char key = '\0';
    Wire.requestFrom(KEYPAD_ADDR, (uint8_t)1);
    if (!key_still_pressed && keypad.isPressed())
    {
        if (keypad.getKey() == I2C_KEYPAD_FAIL){
            return key;
        }
        key = keypad.getLastChar(); // note we want the translated char
        Serial.print(key);
        Serial.println(" pressed.");
    }
    key_still_pressed = keypad.isPressed();
    return key;
}