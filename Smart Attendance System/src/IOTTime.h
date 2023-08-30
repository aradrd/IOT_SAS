#include "time.h"

// NTP server to request epoch time
const char* ntpServer = "time.google.com";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;


class IOTTime{
public:
    IOTTime() = default;
    IOTTime(const IOTTime&) = default;
    IOTTime& operator=(const IOTTime&) = delete;

    void init() {
        Serial.println("Connecting to ntp");
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }

    void printLocalTime()
    {
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }
};
