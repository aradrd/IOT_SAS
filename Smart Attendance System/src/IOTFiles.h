#ifndef IOT_FS_H
#define IOT_FS_H

#include <SPIFFS.h>
#include <mutex>
#include <map>

#include <consts.h>

enum FileLock {
    ATTENDANCELOG_LOCK,
    USERLIST_LOCK,
    PENDINGUSERLIST_LOCK,
};

class IOTFiles {
public:
    IOTFiles() {
        SPIFFS.begin();
    }
    IOTFiles(const IOTFiles&) = delete;
    IOTFiles& operator=(const IOTFiles&) = delete;
    ~IOTFiles() {
        SPIFFS.end();
    };

    void addAttendanceLogEntry(const String& entry);
    void clearAttendanceLog();
    void writeUserList(const UserList& user_list, FileName file_name);
    String debugReadFile(FileName file_name);

    void clearPendingUserList();
    void addPendingUserEntry(const String& id, const String& uid);

   std::vector<String> getChanges(FileName file_name, uint16_t amount_synced);

   void clearAllFiles();

    // For testing
    String readUserList();
    String readPendingUserList();
    String readAttendanceLog();
    bool idExists(const String& id);
    bool uidApproved(const String& uid);

private:
    std::mutex mutexes[AMOUNT_OF_FILES];

    void lock(FileName file_name);
    void unlock(FileName file_name);
    File open(FileName file_name, const String& mode = "r");
    void clearFile(FileName file_name);
    void addEntry(FileName file_name, const String& entry, const bool is_locked = false);
    void addEntry(FileName file_name, const String& id, const String& uid, const bool is_locked = false);
    String readFile(FileName file_name);
    String getLineWithString(FileName file_name, const String& str);
};

#endif
