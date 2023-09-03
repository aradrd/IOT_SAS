#ifndef IOT_FS_H
#define IOT_FS_H

#include <SPIFFS.h>

#include <consts.h>
#include <IOTExceptions.h>
#include <IOTTime.h>

class IOTFiles {
public:
    IOTFiles(IOTTime& iot_time) : iot_time(iot_time) {
    }
    IOTFiles(const IOTFiles&) = delete;
    IOTFiles& operator=(const IOTFiles&) = delete;
    ~IOTFiles() {
        SPIFFS.end();
    };

    void init() {
        SPIFFS.begin();
    }

    String addAttendanceLogEntry(const String& uid) {
        String entry = uid + "," + getTimestamp();
        addEntry(ATTENDANCELOG_PATH, entry);

        Serial.println("Entire file:");
        Serial.println(readFile(ATTENDANCELOG_PATH));

        return entry;
    }

    void clearAttendanceLog() {
        clearFile(ATTENDANCELOG_PATH);
    }

    void clearUserList() {
        clearFile(USERLIST_PATH);
    }

    void addUserEntry(const String& id, const String& uid) {
        String entry = id + "," + uid;
        addEntry(USERLIST_PATH, entry);
    }

private:
    IOTTime& iot_time;

    String getTimestamp() {
        return iot_time.getTime();
    }

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
};

#endif