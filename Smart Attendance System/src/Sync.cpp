#include <Sync.h>
#include <consts.h>

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
    String payload = sheets.getUserList();
    UserList user_list = parseUserList(payload);
    files.writeUserList(user_list);
    Serial.println(files.debugReadFile(USER_LIST));
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

}