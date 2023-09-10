#ifndef IOT_SAS_H
#define IOT_SAS_H

#include <consts.h>
#include <Display.h>
#include <GoogleSheet.h>
#include <IOTFiles.h>
#include <IOTKeypad.h>
#include <IOTTime.h>
#include <RFID.h>

class SmartAttendanceSystem {
public:
    SmartAttendanceSystem()
        : time(), files(),
          display(&Wire, DISPLAY_WIDTH, DISPLAY_HEIGHT),
          keypad(), sheets(), rfid() {}
    SmartAttendanceSystem(const SmartAttendanceSystem&) = delete;
    const SmartAttendanceSystem& operator=(const SmartAttendanceSystem&) = delete;
    ~SmartAttendanceSystem() = default;

    void init() {
        keypad.init();
        display.init();
        rfid.init();
        time.init();
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
                display.clear();
                display.println("UID not recognized.\nHave a nice day.");
            }
        }
    }
    
private:
    IOTTime time;
    IOTFiles files;
    Display display;
    IOTKeypad keypad;
    GoogleSheet sheets;
    RFID rfid;

    void handleKeypress(char key) {
        display.print(key);
    }

    String createLogEntry(const String& uid) {
        return uid + "," + time.getTimeStamp();
    }
};

#endif