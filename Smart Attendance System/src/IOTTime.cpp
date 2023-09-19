#include <IOTTime.h>
#include <time.h>
#include <Arduino.h>

const uint8_t TIMESTAMP_BUFFER_SIZE = 80;
const char* TIMESTAMP_FORMAT = "%Y/%m/%d %H:%M:%S";
const char* NTP_SERVER = "time.google.com";
const long  gmtOffset_sec = 2 * 3600;
const int   daylightOffset_sec = 3600;

bool IOTTime::init() {
    configTime(gmtOffset_sec, daylightOffset_sec, NTP_SERVER);
    return getLocalTime(&timeinfo);
}

String IOTTime::getTimeStamp() {
    if (!getLocalTime(&timeinfo)) {
        if (IOT_DEBUG) {
            Serial.println("Failed getting time");
        }
    }
    char buffer[TIMESTAMP_BUFFER_SIZE];
    std::strftime(buffer, sizeof(buffer), TIMESTAMP_FORMAT, &timeinfo);
    return buffer;
}

void IOTTime::printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        if (IOT_DEBUG) {
            Serial.println("Failed to obtain time");
        }
        return;
    }
    if (IOT_DEBUG) {
        Serial.println(&timeinfo, TIMESTAMP_FORMAT);
    }
}