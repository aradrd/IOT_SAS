#include <IOTFiles.h>

std::map<FileName, String> FILE_TO_PATH = {
    {ATTENDANCE_LOG, ATTENDANCELOG_PATH},
    {USER_LIST, USERLIST_PATH}
};

std::map<FileName, uint8_t> FILE_TO_LOCK = {
    {ATTENDANCE_LOG, ATTENDANCELOG_LOCK},
    {USER_LIST, USERLIST_LOCK}
};


void IOTFiles::addAttendanceLogEntry(const String& entry) {
    addEntry(ATTENDANCE_LOG, entry);
}

void IOTFiles::clearAttendanceLog() {
    clearFile(ATTENDANCE_LOG);
}

void IOTFiles::writeUserList(const UserList& user_list) {
    lock(USER_LIST);
    clearUserList();
    for (const auto& user : user_list) {
        addUserEntry(user.first, user.second);
    }
    unlock(USER_LIST);
}

String IOTFiles::debugReadFile(FileName file_name) {
    return readFile(file_name);
}

void IOTFiles::lock(FileName file_name) {
    mutexes[FILE_TO_LOCK[file_name]].lock();
}

void IOTFiles::unlock(FileName file_name) {
    mutexes[FILE_TO_LOCK[file_name]].unlock();
}

void IOTFiles::clearUserList() {
    clearFile(USER_LIST);
}

void IOTFiles::addUserEntry(const String& id, const String& uid) {
    String entry = id + "," + uid;
    addEntry(USER_LIST, entry);
}

File IOTFiles::open(FileName file_name, const String& mode) {
    const String& path = FILE_TO_PATH[file_name];
    File file = SPIFFS.open(path, mode.c_str());
    if (!file) {
        Serial.println("Error opening file " + path);
    }
    return file;
}

void IOTFiles::clearFile(FileName file_name) {
    File file = open(file_name, "w");
    file.close();
}

void IOTFiles::addEntry(FileName file_name, const String& entry) {
    File file = open(file_name, "a");
    file.println(entry);
    file.close();
}

String IOTFiles::readFile(FileName file_name) {
    File file = open(file_name);
    String res;
    while (file.available()) {
        res += static_cast<char>(file.read());
    }
    return res;
}