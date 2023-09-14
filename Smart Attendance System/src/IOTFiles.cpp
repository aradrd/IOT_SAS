#include <IOTFiles.h>
#include <vector>

const std::map<FileName, String> FILE_TO_PATH = {
    {ATTENDANCE_LOG, ATTENDANCELOG_PATH},
    {USER_LIST, USERLIST_PATH},
    {PENDING_USER_LIST, PENDINGUSERLIST_PATH},
};

const std::map<FileName, uint8_t> FILE_TO_LOCK = {
    {ATTENDANCE_LOG, ATTENDANCELOG_LOCK},
    {USER_LIST, USERLIST_LOCK},
    {PENDING_USER_LIST, PENDINGUSERLIST_LOCK},
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
        addEntry(USER_LIST, user.id, user.uid, true);
    }
    unlock(USER_LIST);
}

String IOTFiles::debugReadFile(FileName file_name) {
    return readFile(file_name);
}

void IOTFiles::lock(FileName file_name) {
    mutexes[FILE_TO_LOCK.at(file_name)].lock();
}

void IOTFiles::unlock(FileName file_name) {
    mutexes[FILE_TO_LOCK.at(file_name)].unlock();
}

void IOTFiles::clearUserList() {
    clearFile(USER_LIST);
}

File IOTFiles::open(FileName file_name, const String& mode) {
    const String& path = FILE_TO_PATH.at(file_name);
    File file = SPIFFS.open(path, mode.c_str());
    if (!file) {
        Serial.println("Error opening file " + path);
    }
    return file;
}

void IOTFiles::clearAllFiles() {
    if (!SPIFFS.format()) {
        Serial.println("Format failed...");
    }
    else {
        Serial.println("Successfully formatted SPIFFS.");
    }
}

void IOTFiles::clearFile(FileName file_name) {
    File file = open(file_name, "w");
    file.close();
}

void IOTFiles::addEntry(FileName file_name, const String& entry, const bool is_locked) {
    if (!is_locked) {
        lock(file_name);
    }
    File file = open(file_name, "a");
    file.println(entry);
    file.close();
    if (!is_locked) {
        unlock(file_name);
    }
}

void IOTFiles::addEntry(FileName file_name, const String& id, const String& uid, const bool is_locked) {
    addEntry(file_name, id + "," + uid, is_locked);
}

String IOTFiles::readFile(FileName file_name) {
    File file = open(file_name);
    String res;
    while (file.available()) {
        res += static_cast<char>(file.read());
    }
    return res;
}

void IOTFiles::clearPendingUserList() {
    clearFile(PENDING_USER_LIST);
}

void IOTFiles::addPendingUserEntry(const String& id, const String& uid){
    String entry = id + "," + uid;
    addEntry(PENDING_USER_LIST, entry);
}

// For testing
String IOTFiles::readUserList(){
    return readFile(USER_LIST);
}

String IOTFiles::readPendingUserList(){
    return readFile(PENDING_USER_LIST);
}

String IOTFiles::readAttendanceLog(){
    return readFile(ATTENDANCE_LOG);
}

bool IOTFiles::idExists(const String& id){
    return (!getLineWithString(PENDING_USER_LIST, id).isEmpty() ||
            !getLineWithString(USER_LIST, id).isEmpty());
}

bool IOTFiles::uidApproved(const String& uid){
    return (!getLineWithString(USER_LIST, uid).isEmpty());
}

String IOTFiles::getLineWithString(FileName file_name, const String& str){
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

std::vector<String> IOTFiles::getChanges(FileName file_name, uint16_t from_line) {
    lock(file_name);
    File file = open(file_name);
    for (int i = 0; i < from_line; i++) {
        file.readStringUntil('\n');
    }

    std::vector<String> pending_changes;
    while (file.available()) {
        pending_changes.push_back(file.readStringUntil('\n'));
    }

    unlock(file_name);
    return pending_changes;
}
