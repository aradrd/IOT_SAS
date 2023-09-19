#ifndef IOT_DISPLAY_H
#define IOT_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>

#include <consts.h>

class Display {
public:
    Display(TwoWire *wire, const uint8_t width, const uint8_t height)
        : wire(wire), width(width), height(height),
          display(width, height, wire, DISPLAY_RST), currently_displayed("")
    {}

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
        clear();
        println("Waiting\nfor\nNTP...");
    }

    void print(const char& ch) {
        print(static_cast<String>(ch));
    }

    void print(const String& str) {
        currently_displayed.concat(str);
        display.print(str);
        display.display();
    }

    void println(const String& str) {
        print(str + "\n");
    }

    void backspace(){
        currently_displayed.remove(currently_displayed.length() - 1);
        String temp = currently_displayed;
        clear();
        currently_displayed = "";
        print(temp);
    }

    void clear() {
        currently_displayed = "";
        drawWifiStatus();
    }

    void drawWifiStatus(){
        display.setCursor(0, 0);
        display.clearDisplay();
        if(!currently_displayed.isEmpty()){
            display.print(currently_displayed);
        }
        if (WiFi.status() == WL_CONNECTED) {
            display.drawBitmap(110,0,wifiicon,8,8,WHITE);  
        }
        else{
            display.drawBitmap(110,0,nowifiicon,8,8,WHITE);
        }
        display.display();
    }

    String getDisplayed(){
        return currently_displayed;
    }

    void displayWithDelay(const String& str, const uint16_t to_delay=MSG_DELAY){
        clear();
        println(str);
        delay(to_delay); // TODO - delay is not good, do with timer
    }

    void blink(){
        clear();
        display.invertDisplay(true);
        delay(DISPLAY_BLINK_DELAY); // TODO - delay is not good, do with timer
        display.invertDisplay(false);
    }

private:
    TwoWire* wire;
    const uint8_t width;
    const uint8_t height;
    Adafruit_SSD1306 display;
    String currently_displayed;
};

#endif