#ifndef IOT_SYNC_H
#define IOT_SYNC_H

#include <mutex>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <map>

#include <GoogleSheet.h>
#include <IOTFiles.h>

const std::map<FileName, String> FILE_TO_PREF {
    {ATTENDANCE_LOG, "attendance_log"},
    {PENDING_USER_LIST, "pending_user_list"},
    {USER_LIST, "user_list"},
};

class Sync {
public:
    Sync(GoogleSheet& sheets, IOTFiles& files) : sheets(sheets), files(files), preferences() {
        preferences.begin("sync_status", false);
    }
    Sync(const Sync&) = delete;
    Sync& operator=(const Sync&) = delete;
    ~Sync() {
        preferences.end();
    }

    void sync();

private:
    std::mutex mutex;
    GoogleSheet& sheets;
    IOTFiles& files;
    Preferences preferences;

    void pull();
    UserList parseUserList(const String& payload);
    void push();
    void pushChanges(FileName);
};

#endif