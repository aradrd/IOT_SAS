#ifndef IOT_TIME_H
#define IOT_TIME_H

#include <iostream>
#include <ctime>

#include <IOTExceptions.h>

// NTP server to request epoch time
const uint8_t TIMESTAMP_BUFFER_SIZE = 80;
const char* TIMESTAMP_FORMAT = "%Y/%m/%d %H:%M:%S";
const char* NTP_SERVER = "time.google.com";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;


class IOTTime{
public:
    IOTTime() : timeinfo() {}
    IOTTime(const IOTTime&) = default;
    IOTTime& operator=(const IOTTime&) = delete;

    void init() {
        configTime(gmtOffset_sec, daylightOffset_sec, NTP_SERVER);
    }

    String getTime() {
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed getting time");
            // throw TimeError();
        }
        char buffer[TIMESTAMP_BUFFER_SIZE];
        std::strftime(buffer, sizeof(buffer), TIMESTAMP_FORMAT, &timeinfo);
        return buffer;
    }

    void printLocalTime() {
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
        Serial.println(&timeinfo, TIMESTAMP_FORMAT);
    }

private:
    struct tm timeinfo;
};

#endif