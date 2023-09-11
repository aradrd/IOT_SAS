#include <SmartAttendanceSystem.h>

void SmartAttendanceSystem::init() {
    keypad.init();
    display.init();
    rfid.init();
    time.init();
}

void SmartAttendanceSystem::tick() {
    char key = keypad.tick();
    if (key != '\0') {
        handleKeypress(key);
    }

    String uid = rfid.tick();
    if (uid != "") {
        Serial.println("Found RFID: " + uid);
        if (uid == "A7 48 B3 4E") {
            String entry = createLogEntry(uid);
            files.addAttendanceLogEntry(entry);
            sheets.addAttendanceLogEntry(entry);
        }
        else {
            display.clear();
            display.println("UID not recognized.\nHave a nice day.");
        }
    }
    if (ticks % (100 * 60) == 0) {
        xTaskCreate(sync_thread, "sync", STACK_SIZE, static_cast<void*>(this), 1, nullptr);
    }
    ticks++;
}

void SmartAttendanceSystem::sync() {
    sync.sync();
}

void SmartAttendanceSystem::handleKeypress(char key) {
    display.print(key);
}

String SmartAttendanceSystem::createLogEntry(const String& uid) {
    return uid + "," + time.getTimeStamp();
}
