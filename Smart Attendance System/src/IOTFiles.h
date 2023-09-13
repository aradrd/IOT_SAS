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

    void clearPendingUserList() {
        clearFile(PENDING_USER_LIST);
    }

    void addUserEntry(const String& id, const String& uid) {
        String entry = id + "," + uid;
        addEntry(USER_LIST, entry);
    }

    void addPendingUserEntry(const String& id, const String& uid){
        String entry = id + "," + uid;
        addEntry(PENDING_USER_LIST, entry);
    }

    // For testing
    String readUserList(){
        return readFile(USER_LIST);
    }

    String readPendingUserList(){
        return readFile(PENDING_USER_LIST);
    }

    String readAttendanceLog(){
        return readFile(ATTENDANCE_LOG);
    }

    bool idExists(String id){
        return (!getLineWithString(PENDING_USER_LIST, id).isEmpty() ||
                !getLineWithString(USER_LIST, id).isEmpty());
    }

    bool uidApproved(String uid){
        return (!getLineWithString(USER_LIST, uid).isEmpty());
    }

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

    String getLineWithString(FileName file_name, const String& str){
        File file = open(file_name);
        String line;
        size_t size = file.size();
        line.reserve(size);

        line = file.readStringUntil('\n');
        while(!line.isEmpty()){
            // check if line includes id
            if(line.indexOf(str) != -1 ){
                //found a line, return it
                file.close();
                return line;
            }
            // read next line
            line = file.readStringUntil('\n');
        }
        file.close();
        return "";
    }
};

#endif
