#ifndef IOT_SAS_H
#define IOT_SAS_H

#include <consts.h>
#include <Display.h>
#include <GoogleSheet.h>
#include <IOTFiles.h>
#include <IOTKeypad.h>
#include <IOTTime.h>
#include <RFID.h>
#include <Sync.h>

typedef enum {
    WAIT_FOR_CARD = 0,
    CARD_SWIPED = 1,
    CARD_REGISTERED = 2,
    CARD_APPROVED = 3,
    WAIT_FOR_ID = 4,
    CARD_NOT_REGISTERED = 5,
} states;

class SmartAttendanceSystem {
public:
    SmartAttendanceSystem()
        : time(), files(),
          display(&Wire, DISPLAY_WIDTH, DISPLAY_HEIGHT),
          keypad(), sheets(), rfid(), state(WAIT_FOR_CARD), is_first_digit(true),
          uid_in_registration(), sync(sheets, files) {}
    SmartAttendanceSystem(const SmartAttendanceSystem&) = delete;
    const SmartAttendanceSystem& operator=(const SmartAttendanceSystem&) = delete;
    ~SmartAttendanceSystem() = default;

    void init();
    void tick();
    
private:
    IOTTime time;
    IOTFiles files;
    Display display;
    IOTKeypad keypad;
    GoogleSheet sheets;
    RFID rfid;
    String createLogEntry(const String& uid);
    states state;
    bool is_first_digit;
    String uid_in_registration;
    Sync sync;
    time_t last_action = 0;
    time_t last_sync_call = 0;

    void handleKeypress(char key);
    void tickWaitForCard();
    void tickCardNotRegistered();
    void tickWaitForId();
    bool isTimeout();
    void resetTimeUntouched();
    void validateAndSendId();
    void resetState();
    void callSync();
    void factoryReset();
};

#endif
