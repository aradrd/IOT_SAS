#ifndef IOT_SAS_H
#define IOT_SAS_H

#include <consts.h>
#include <Display.h>
#include <GoogleSheet.h>
#include <IOTFiles.h>
#include <IOTKeypad.h>
#include <IOTTime.h>
#include <RFID.h>

enum {
    MSG = 0,
    ID = 1
};

class SmartAttendanceSystem {
public:
    SmartAttendanceSystem()
        : time(), files(),
          display({Display(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT),
                   Display(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT)}),
          keypad(), sheets(), rfid() {}
    SmartAttendanceSystem(const SmartAttendanceSystem &) = delete;
    const SmartAttendanceSystem& operator=(const SmartAttendanceSystem&) = delete;
    ~SmartAttendanceSystem() = default;

    void init() {
        keypad.init();
        display[MSG].init();
        display[ID].init();
        rfid.init();
        time.init();

        sheets.readDataFromGoogleSheet();
    }

    void tick() {
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
                display[MSG].clear();
                display[MSG].println("UID not recognized.\nHave a nice day.");
                uid = "";
            }
        }

        bool something_happened = key != '\0' || uid != "";
        if (something_happened) {
            display[MSG].clear();
        }
    }
    
private:
    IOTTime time;
    IOTFiles files;
    Display display[2];
    IOTKeypad keypad;
    GoogleSheet sheets;
    RFID rfid;

    void handleKeypress(char key) {
        display[ID].print(key);
    }

    String createLogEntry(const String& uid) {
        return uid + "," + time.getTimeStamp();
    }
};

#endif