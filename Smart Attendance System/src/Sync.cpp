#include <Sync.h>
#include <consts.h>

void Sync::init() {
    preferences.begin("sync_status", false);
}

void Sync::sync() {
    if (!mutex.try_lock()) {
        Serial.println("Thread already running.");
        return;
    }

    pull();
    push();

    mutex.unlock();
}

void Sync::pull() {
    Serial.println("Pulling...");
    String payload = sheets.getUserList();
    Serial.println("Got userlist");
    UserList user_list = parseUserList(payload);
    Serial.println("Parsed userlist");
    files.writeUserList(user_list);
    Serial.println("Wrote userlist");
    Serial.println(files.debugReadFile(USER_LIST));
    Serial.println("Finished pulling.");
}

UserList Sync::parseUserList(const String& payload) {
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, payload);
    const JsonArray& data = doc["data"];
    UserList user_list;
    for (const JsonArray& user : data) {
        String ID = user[0];
        String UID = user[1];
        user_list.push_back({ID, UID});
    }
    return user_list;
}

void Sync::push() {
    pushChanges(PENDING_USER_LIST);
    pushChanges(ATTENDANCE_LOG);
}

void Sync::pushChanges(FileName file_name) {
    Serial.println("Pushing...");
    const String& pref_name = FILE_TO_PREF.at(file_name);
    uint16_t last_synced_line = preferences.getUShort(pref_name.c_str(), 0);
    std::vector<String> pending_changes = files.getChanges(file_name, last_synced_line);
    for (const auto& entry : pending_changes) {
        Serial.println("Change: " + entry);
    }
    Serial.println("Calling postChanges for " + pref_name);
    Serial.println("last_synced_line before: " + String(last_synced_line));
    if (pending_changes.size() > 0) {
        last_synced_line += sheets.postChanges(file_name, pending_changes);
    }
    else {
        Serial.println("Nothing changed, aborting post.");
    }
    preferences.putUShort(pref_name.c_str(), last_synced_line);
    Serial.println("last_synced_line after: " + String(preferences.getUShort(pref_name.c_str(), 0)));
    Serial.println("Finished pushing.");
}

void Sync::clearSyncStatus() {
    if (preferences.clear()) {
        Serial.println("Cleared preferences.");
    }
    else {
        Serial.println("Failed clearing preferences.");
    }
}