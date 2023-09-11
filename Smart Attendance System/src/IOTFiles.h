#ifndef IOT_FS_H
#define IOT_FS_H

#include <SPIFFS.h>

#include <consts.h>
#include <IOTExceptions.h>

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
        addEntry(ATTENDANCELOG_PATH, entry);
    }

    void clearAttendanceLog() {
        clearFile(ATTENDANCELOG_PATH);
    }

    void clearUserList() {
        clearFile(USERLIST_PATH);
    }

    void clearPendingUserList() {
        clearFile(PENDINGUSERLIST_PATH);
    }

    void addUserEntry(const String& id, const String& uid) {
        String entry = id + "," + uid;
        addEntry(USERLIST_PATH, entry);
    }

    void addPendingUserEntry(const String& id, const String& uid){
        String entry = id + "," + uid;
        addEntry(PENDINGUSERLIST_PATH, entry);
    }

    // For testing
    String readUserList(){
        return readFile(USERLIST_PATH);
    }

    String readPendingUserList(){
        return readFile(PENDINGUSERLIST_PATH);
    }

    String readAttendanceLog(){
        return readFile(ATTENDANCELOG_PATH);
    }

    bool idExists(String id){
        return (!getLineWithString(PENDINGUSERLIST_PATH, id).isEmpty() ||
                !getLineWithString(USERLIST_PATH, id).isEmpty());
    }

    bool uidApproved(String uid){
        return (!getLineWithString(USERLIST_PATH, uid).isEmpty());
    }

private:
    File open(const String& path, const String& mode = "r") {
        File file = SPIFFS.open(path, mode.c_str());
        if (!file) {
            throw OpenFileError(path);
        }
        return file;
    }

    void clearFile(const String& path) {
        File file = open(path, "w");
        file.close();
    }

    void addEntry(const String& path, const String& entry) {
        File file = open(path, "a");
        Serial.println("Opened file");
        int written = writeString(file, entry);
        file.close();
        Serial.println("Closed file");
        if (written != entry.length()) {
            // throw WriteError(file.path(), written, entry.length());
            Serial.println("Write length different");
        }
    }

    int writeString(File& file, const String& str) {
        int write_len = 0;
        for (size_t i = 0; i < str.length(); i++) {
            write_len += file.write(str[i]);
        }
        return write_len;
    }

    String readFile(const String& path) {
        File file = open(path);
        String res;
        size_t size = file.size();
        res.reserve(size);
        for (size_t i = 0; i < size; i++) {
            res += static_cast<char>(file.read());
        }
        file.close();
        return res;
    }

    /* Searchs for a string, line by line */
    String getLineWithString(const String& path, const String& str){
        File file = open(path);
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