#ifndef IOT_SYNC_H
#define IOT_SYNC_H

#include <mutex>
#include <ArduinoJson.h>

#include <GoogleSheet.h>
#include <IOTFiles.h>

class Sync {
public:
    Sync(GoogleSheet& sheets, IOTFiles& files) : sheets(sheets), files(files) {}
    Sync(const Sync&) = delete;
    Sync& operator=(const Sync&) = delete;

    void sync();

private:
    std::mutex mutex;
    GoogleSheet& sheets;
    IOTFiles& files;
    uint8_t last_index;

    void pull();
    UserList parseUserList(const String& payload);
    void push();
};

#endif