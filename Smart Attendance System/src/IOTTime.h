#ifndef IOT_TIME_H
#define IOT_TIME_H

#include <iostream>
#include <ctime>
#include <WString.h>

class IOTTime {
public:
    IOTTime() : timeinfo() {}
    IOTTime(const IOTTime&) = default;
    IOTTime& operator=(const IOTTime&) = delete;

    void init();
    String getTimeStamp();
    void printLocalTime();

private:
    struct tm timeinfo;
};

#endif