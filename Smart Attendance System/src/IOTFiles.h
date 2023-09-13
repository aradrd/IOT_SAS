#ifndef IOT_FS_H
#define IOT_FS_H

#include <SPIFFS.h>
#include <mutex>
#include <map>

#include <consts.h>

enum FileLock {
    ATTENDANCELOG_LOCK,
    USERLIST_LOCK
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
    void writeUserList(const UserList& user_list);
    String debugReadFile(FileName file_name);

private:
    std::mutex mutexes[2];

    void lock(FileName file_name);
    void unlock(FileName file_name);
    void clearUserList();
    void addUserEntry(const String& id, const String& uid);
    File open(FileName file_name, const String& mode = "r");
    void clearFile(FileName file_name);
    void addEntry(FileName file_name, const String& entry);
    String readFile(FileName file_name);
};

#endif