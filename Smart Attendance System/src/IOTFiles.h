#ifndef IOT_FS_H
#define IOT_FS_H

#include <SPIFFS.h>
#include <mutex>
#include <map>

#include <consts.h>
#include <IOTExceptions.h>

enum FileLock {
    ATTENDANCELOG_LOCK,
    USERLIST_LOCK
};

std::map<FileName, String> FILE_TO_PATH = {
    {ATTENDANCE_LOG, ATTENDANCELOG_PATH},
    {USER_LIST, USERLIST_PATH}
};

std::map<FileName, uint8_t> FILE_TO_LOCK = {
    {ATTENDANCE_LOG, ATTENDANCELOG_LOCK},
    {USER_LIST, USERLIST_LOCK}
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

    void addAttendanceLogEntry(const String& entry) {
        addEntry(ATTENDANCE_LOG, entry);
    }

    void clearAttendanceLog() {
        clearFile(ATTENDANCE_LOG);
    }

    void writeUserList(const UserList& user_list) {
        lock(USER_LIST);
        clearUserList();
        for (const auto& user : user_list) {
            addUserEntry(user.first, user.second);
        }
        unlock(USER_LIST);
    }

    String debugReadFile(FileName file_name) {
        return readFile(file_name);
    }

private:
    std::mutex mutexes[2];

    void lock(FileName file_name) {
        mutexes[FILE_TO_LOCK[file_name]].lock();
    }

    void unlock(FileName file_name) {
        mutexes[FILE_TO_LOCK[file_name]].unlock();
    }

    void clearUserList() {
        clearFile(USER_LIST);
    }

    void addUserEntry(const String& id, const String& uid) {
        String entry = id + "," + uid;
        addEntry(USER_LIST, entry);
    }

    File open(FileName file_name, const String& mode = "r") {
        const String& path = FILE_TO_PATH[file_name];
        File file = SPIFFS.open(path, mode.c_str());
        if (!file) {
            Serial.println("Error opening file " + path);
        }
        return file;
    }

    void clearFile(FileName file_name) {
        File file = open(file_name, "w");
        file.close();
    }

    void addEntry(FileName file_name, const String& entry) {
        File file = open(file_name, "a");
        file.println(entry);
        file.close();
    }

    String readFile(FileName file_name) {
        File file = open(file_name);
        String res;
        while (file.available()) {
            res += static_cast<char>(file.read());
        }
        return res;
    }
};

#endif