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
          keypad(), sheets(), rfid(), state(WAIT_FOR_CARD), is_first_digit(true) {}
    SmartAttendanceSystem(const SmartAttendanceSystem&) = delete;
    const SmartAttendanceSystem& operator=(const SmartAttendanceSystem&) = delete;
    ~SmartAttendanceSystem() = default;

    void init() {
        keypad.init();
        display.init();
        rfid.init();
        time.init();

        reset_state();
    }

    void tick() {
        switch(state){
            case WAIT_FOR_CARD: 
                tick_wait_for_card();
                break;
            case CARD_NOT_REGISTERED: 
                tick_card_not_registerd();
                break;
            case WAIT_FOR_ID: 
                tick_wait_for_id();
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

    String createLogEntry(const String& uid) {
        return uid + "," + time.getTimeStamp();
    }
    
    void tick_wait_for_card(){
        String uid = rfid.tick();
        if (uid != "") {
            Serial.println("Found RFID: " + uid);
            if (uid == "A7 48 B3 4E") {
                // card is registered and approved
                state = CARD_APPROVED;
                String entry = createLogEntry(uid);
                files.addAttendanceLogEntry(entry);
                sheets.addAttendanceLogEntry(entry);
                state = WAIT_FOR_CARD;
            }
            else {
                // card is not reistered
                state = CARD_NOT_REGISTERED;
                display.clear();
                display.println("Press A for registration or C for cancel.");
                reset_time_untouched();
            }
        }
    }

    void tick_card_not_registerd(){
        if(is_timeout()){
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
                reset_time_untouched();
                break;
            case CANCEL_KEY:
                reset_state();
                break;
            default:
                display.clear();
                display.println("Wrong key.\nPress A for registration or C for cancel.");
                break;
        }
    }

    void tick_wait_for_id(){
        if(is_timeout()){
            return;
        }
        char key = keypad.tick();

        switch(key){
            case '\0':
                return;
            case OK_KEY:
                validate_and_send_id();
                break;
            case BACKSPACE_KEY:
                reset_time_untouched();
                display.backspace();
                break;
            case CANCEL_KEY:
                reset_state();
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
                if (display.get_currently_displayed().length() < ID_LENGTH){
                    reset_time_untouched();
                    display.print(key);
                }
        }
    }
  
    bool is_timeout(){
        if( micros() - start_time > MICROS_TIMEOUT){
            display.clear();
            display.println("Timeout passed.");
            delay(MSG_DELAY);
            reset_state();
            return true;
        }
        return false;
    }
    
    void reset_time_untouched(){
        start_time = micros();
    }

    void validate_and_send_id(){
        String id = display.get_currently_displayed();
        if (id.length() == ID_LENGTH){
            Serial.println("Got " + id);
            display.clear();
            display.println("Sending registration to approval...");
            // TODO: do something with id
            delay(MSG_DELAY);
            reset_state();
        }
        else{
            Serial.println("ID is not valid");
            delay(MSG_DELAY);
            reset_state();
        }
    }

    void reset_state(){
        state = WAIT_FOR_CARD;
        display.clear();
        display.println("Please swipe card");
    }

};

#endif