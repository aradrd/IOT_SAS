#ifndef IOT_SYNC_H
#define IOT_SYNC_H

#include <mutex>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <map>

#include <GoogleSheet.h>
#include <IOTFiles.h>

// Preferences keys have a max length of 15 characters.
const std::map<FileName, String> FILE_TO_PREF {
    {ATTENDANCE_LOG, "log"},
    {PENDING_USER_LIST, "p_user_list"},
    {USER_LIST, "user_list"},
};

class Sync {
public:
    Sync(GoogleSheet& sheets, IOTFiles& files) : sheets(sheets), files(files), preferences()
    {}
    Sync(const Sync&) = delete;
    Sync& operator=(const Sync&) = delete;
    ~Sync() {
        preferences.end();
    }

    void init();
    void sync();
    void clearSyncStatus();

private:
    std::mutex mutex;
    GoogleSheet& sheets;
    IOTFiles& files;
    Preferences preferences;

    bool pull();
    UserList parseUserList(const String& payload, const String& list);
    bool push();
    bool pushChanges(FileName file_name);
};

#endif