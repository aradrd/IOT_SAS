#ifndef IOT_SAS_H
#define IOT_SAS_H

#include <consts.h>
#include <Display.h>
#include <GoogleSheet.h>
#include <IOTFiles.h>
#include <IOTKeypad.h>
#include <IOTTime.h>
#include <RFID.h>

typedef enum{
    WAIT_FOR_CARD = 0,
    CARD_SWIPED = 1,
    CARD_REGISTERED = 2,
    CARD_APPROVED = 3,
    WAIT_FOR_ID = 4,
    CARD_NOT_REGISTERED = 5,
}states;

class SmartAttendanceSystem {
public:
    SmartAttendanceSystem()
        : time(), files(),
          display(&Wire, DISPLAY_WIDTH, DISPLAY_HEIGHT),
          keypad(), sheets(), rfid(), state(WAIT_FOR_CARD), is_first_digit(true),
          uid_in_registration() {}
    SmartAttendanceSystem(const SmartAttendanceSystem&) = delete;
    const SmartAttendanceSystem& operator=(const SmartAttendanceSystem&) = delete;
    ~SmartAttendanceSystem() = default;

    void init() {
        keypad.init();
        display.init();
        rfid.init();
        time.init();

        resetState();
    }

    void tick() {
        switch(state){
            case WAIT_FOR_CARD: 
                tickWaitForCard();
                break;
            case CARD_NOT_REGISTERED: 
                tickCardNotRegistered();
                break;
            case WAIT_FOR_ID: 
                tickWaitForId();
                break;
        }
    }
  
private:
    IOTTime time;
    IOTFiles files;
    Display display;
    IOTKeypad keypad;
    GoogleSheet sheets;
    RFID rfid;
    states state;
    bool is_first_digit;
    long start_time;
    String uid_in_registration;

    String createLogEntry(const String& uid) {
        return uid + "," + time.getTimeStamp();
    }
    
    void tickWaitForCard(){
        String uid = rfid.tick();
        if (uid != "") {
            Serial.println("Found RFID: " + uid);
            // check if approved
            if (files.uidApproved(uid)) {
                Serial.println("Card Approved");
                Serial.println(files.readUserList());
                display.blink();
                state = CARD_APPROVED;
                //String entry = createLogEntry(uid);
                //files.addAttendanceLogEntry(entry);
                //sheets.addAttendanceLogEntry(entry);
                resetState();
            }
            // check if waiting for approval
            else if (files.idExists(uid)){
                display.displayForSeconds("Card is waiting for manager approval.");
                resetState();
            }
            else{
                // card is not registered
                uid_in_registration = uid;
                state = CARD_NOT_REGISTERED;
                display.clear();
                display.println("Press A for registration or C for cancel.");
                resetTimeUntouched();
            }
        }
    }

    void tickCardNotRegistered(){
        if(isTimeout()){
            return;
        }
        char key = keypad.tick();
        switch( key ){
            case '\0':
                return;
            case OK_KEY:
                state = WAIT_FOR_ID;
                display.clear();
                display.println("Please enter a 9 digit ID.\nB for backspace");
                is_first_digit = true;
                resetTimeUntouched();
                break;
            case CANCEL_KEY:
                resetState();
                break;
            default:
                display.clear();
                display.println("Wrong key.\nPress A for registration or C for cancel.");
                break;
        }
    }

    void tickWaitForId(){
        if(isTimeout()){
            return;
        }
        char key = keypad.tick();

        switch(key){
            case '\0':
                return;
            case OK_KEY:
                validateAndSendId();
                break;
            case BACKSPACE_KEY:
                resetTimeUntouched();
                display.backspace();
                break;
            case CANCEL_KEY:
                resetState();
                break;
            case '#':
                break;
            case '*':
                break;
            case 'D':
                break;
            default:
                if(is_first_digit){
                    is_first_digit = false;
                    display.clear();
                }
                if (display.getDisplayed().length() < ID_LENGTH){
                    resetTimeUntouched();
                    display.print(key);
                }
        }
    }
  
    bool isTimeout(){
        if( micros() - start_time > MICROS_TIMEOUT){
            display.displayForSeconds("Timeout passed.");
            resetState();
            return true;
        }
        return false;
    }
    
    void resetTimeUntouched(){
        start_time = micros();
    }

    void validateAndSendId(){
        String id = display.getDisplayed();
        if (id.length() == ID_LENGTH){
            // check if ID already exists
            if(files.idExists(id)){
                display.displayForSeconds("Id already registered to another card.");
                resetState();
            }
            else{
                files.addPendingUserEntry(id, uid_in_registration);
                Serial.println("Got " + id);
                display.displayForSeconds("Sending registration to approval...");
                resetState();
                //TODO - remove, for testing
                Serial.println(files.readPendingUserList());
            }
        }
        else{
            display.displayForSeconds("ID is not valid");
            resetState();
        }
    }

    void resetState(){
        state = WAIT_FOR_CARD;
        display.clear();
        display.println("Please swipe card");
    }

};

#endif