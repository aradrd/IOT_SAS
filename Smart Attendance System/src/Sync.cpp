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

    while (!push()) {
        Serial.println("Failed pushing.");
        sheets.validateWiFi(true);
    }

    while (!pull()) {
        Serial.println("Failed pulling.");
        sheets.validateWiFi(true);
    }

    mutex.unlock();
}

bool Sync::pull() {
    Serial.println("Pulling...");
    String payload = sheets.getUserList();
    if (payload == "") {
        Serial.println("Communication failed.");
        return false;
    }
    UserList approved_user_list = parseUserList(payload, "approved");
    UserList pending_user_list = parseUserList(payload, "pending");
    Serial.println("approved: " + String(approved_user_list.size()));
    Serial.println("pending: " + String(pending_user_list.size()));
    files.writeUserList(approved_user_list, USER_LIST);
    files.writeUserList(pending_user_list, PENDING_USER_LIST);
    preferences.putUShort(FILE_TO_PREF.at(PENDING_USER_LIST).c_str(), pending_user_list.size());
    Serial.println("Finished pulling.");
    return true;
}

UserList Sync::parseUserList(const String& payload, const String& list) {
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, payload);
    const JsonArray& data = doc[list];
    UserList user_list;
    for (const JsonArray& user : data) {
        String ID = user[0];
        String UID = user[1];
        user_list.push_back({ID, UID});
    }
    return user_list;
}

bool Sync::push() {
    return pushChanges(PENDING_USER_LIST) && pushChanges(ATTENDANCE_LOG);
}

bool Sync::pushChanges(FileName file_name) {
    Serial.println("Pushing...");
    const String& pref_name = FILE_TO_PREF.at(file_name);
    uint16_t amount_synced = preferences.getUShort(pref_name.c_str(), 0);
    uint16_t pushed;
    std::vector<String> pending_changes = files.getChanges(file_name, amount_synced);
    for (const auto& entry : pending_changes) {
        Serial.println("Change: " + entry);
    }
    Serial.println("Calling postChanges for " + pref_name);
    Serial.println("amount_synced before: " + String(amount_synced));
    if (pending_changes.size() > 0) {
        pushed = sheets.postChanges(file_name, pending_changes);
    }
    else {
        Serial.println("Nothing changed, aborting post.");
        return true;
    }

    if (pushed > 0) {
        amount_synced += pushed;
        preferences.putUShort(pref_name.c_str(), amount_synced);
        Serial.println("amount_synced after: " + String(preferences.getUShort(pref_name.c_str(), 0)));
        Serial.println("Finished pushing.");
    }
    else {
        Serial.println("Changes for " + pref_name + " weren't pushed.");
    }

    return pushed > 0;
}

void Sync::clearSyncStatus() {
    if (preferences.clear()) {
        Serial.println("Cleared preferences.");
    }
    else {
        Serial.println("Failed clearing preferences.");
    }
}