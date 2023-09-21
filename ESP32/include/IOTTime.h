#ifndef IOT_TIME_H
#define IOT_TIME_H

#include <iostream>
#include <consts.h>
#include <ctime>
#include <WString.h>

class IOTTime {
public:
    IOTTime() : timeinfo() {}
    IOTTime(const IOTTime&) = default;
    IOTTime& operator=(const IOTTime&) = delete;

    bool init();
    String getTimeStamp();
    void printLocalTime();

private:
    struct tm timeinfo;
};

#endif
