#ifndef IOT_SYNC_H
#define IOT_SYNC_H

#include <mutex>
#include <ArduinoJson.h>
#include <vector>

#include <consts.h>
#include <GoogleSheet.h>
#include <IOTFiles.h>

class Sync {
public:
    Sync(GoogleSheet& sheets, IOTFiles& files) : sheets(sheets), files(files) {}
    Sync(const Sync&) = delete;
    Sync& operator=(const Sync&) = delete;

    void sync() {
        if (!mutex.try_lock()) {
            return;
        }

        pull();
        push();

        mutex.unlock();
    }

private:
    std::mutex mutex;
    GoogleSheet& sheets;
    IOTFiles& files;
    uint8_t last_index;

    void pull() {
        String payload = sheets.getUserList();
        UserList user_list = parseUserList(payload);
        files.writeUserList(user_list);
    }

    UserList parseUserList(const String& payload) {
        DynamicJsonDocument doc(JSON_SIZE);
        DeserializationError error = deserializeJson(doc, payload);
        JsonArray data = doc["data"];
        UserList user_list;
        for (const JsonArray& user : data) {
            String ID = user[0];
            String UID = user[1];
            user_list.push_back({ID, UID});
        }
        return user_list;
    }

    void push() {

    }
};

void sync(void* sync) {
    static_cast<Sync*>(sync)->sync();
    vTaskDelete(nullptr);
}

#endif