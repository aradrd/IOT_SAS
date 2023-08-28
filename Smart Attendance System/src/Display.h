#ifndef IOT_DISPLAY_H
#define IOT_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Exception>

const uint8_t DISPLAY_RST = -1; // -1 means shared (whatever that means).
const uint8_t DISPLAY_ADDR = 0x3C;
const uint8_t DISPLAY_TEXT_SIZE = 1;
const uint8_t DISPLAY_TEXT_COLOR = WHITE;

class Display {
public:
    Display(TwoWire *wire, const uint8_t width, const uint8_t height)
        : wire(wire), width(width), height(height),
          display(width, height, wire, DISPLAY_RST)
    {
    }

    Display(const Display&) = default;
    Display& operator=(const Display&) = default;
    ~Display() = default;

    void init() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR))
        {
            Serial.println(F("SSD1306 allocation failed"));
            return;
        }

        display.setTextSize(DISPLAY_TEXT_SIZE);
        display.setTextColor(DISPLAY_TEXT_COLOR);
        display.setCursor(0, 0);
        clear();
    }

    void print(const char& ch) {
        print(static_cast<String>(ch));
    }

    void print(const String& str) {
        display.print(str);
        display.display();
    }

    void println(const String& str) {
        print(str + "\n");
    }

    void clear() {
        display.clearDisplay();
        display.display();
    }

private:
    TwoWire* wire;
    const uint8_t width;
    const uint8_t height;
    Adafruit_SSD1306 display;
};

#endif