#include <SmartAttendanceSystem.h>

void sync_thread(void*);

void SmartAttendanceSystem::init() {
    display.init();
    sheets.init(); // Also inits wifi.
    while (!time.init()) {
        sheets.validateWiFi();
    }
    display.clear();
    keypad.init();
    rfid.init();
    sync.init();

    callSync();
    resetState();
}

void SmartAttendanceSystem::tick() {
    display.drawWifiStatus();
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

void SmartAttendanceSystem::handleKeypress(char key) {
    display.print(key);
}

String SmartAttendanceSystem::createLogEntry(const String& uid) {
    return uid + "," + time.getTimeStamp();
}

void SmartAttendanceSystem::callSync() {
    last_sync_call = millis();
    Serial.println("Calling sync.");
    xTaskCreate(sync_thread, "sync", STACK_SIZE, static_cast<void*>(&sync), 1, nullptr);
}

void SmartAttendanceSystem::factoryReset() {
    files.clearAllFiles();
    sync.clearSyncStatus();
}

void SmartAttendanceSystem::tickWaitForCard(){
    if (millis() - last_sync_call > SYNC_FREQ) {
        callSync();
    }

    // TODO: For debugging, remove later.
    char key = keypad.tick();
    if (key == 'C') {
        factoryReset();
    }
    else if (key == 'D') {
        callSync();
    }
    else if (key == 'B') {
        sync.clearSyncStatus();
    }

    String uid = rfid.tick();
    if (uid != "") {
        Serial.println("Found RFID: " + uid);
        // check if approved
        if (files.uidApproved(uid)) {
            Serial.println("Card Approved");
            display.blink();
            state = CARD_APPROVED;
            String entry = createLogEntry(uid);
            files.addAttendanceLogEntry(entry);
            callSync();
            resetState();
        }
        // check if waiting for approval
        else if (files.idExists(uid)){
            Serial.println("Pending");
            display.displayWithDelay(WAITING_FOR_APPROVAL);
            callSync();
            resetState();
        }
        else{
            // card is not registered
            uid_in_registration = uid;
            state = CARD_NOT_REGISTERED;
            display.clear();
            display.println(REGISTER_MENU);
            resetTimeUntouched();
        }
    }
}

void SmartAttendanceSystem::tickCardNotRegistered(){
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
            display.println(ENTER_ID);
            is_first_digit = true;
            resetTimeUntouched();
            break;
        case CANCEL_KEY:
            resetState();
            break;
        default:
            display.clear();
            display.displayWithDelay(INVALID_KEY, SHORT_MSG_DELAY);
            display.clear();
            display.println(REGISTER_MENU);
            resetTimeUntouched();
            break;
    }
}

void SmartAttendanceSystem::tickWaitForId(){
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
            if(!is_first_digit){
                display.backspace();
            }
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

bool SmartAttendanceSystem::isTimeout(){
    if( millis() - last_action > MILLIS_TIMEOUT){
        display.displayWithDelay(TIMEOUT, SHORT_MSG_DELAY);
        resetState();
        return true;
    }
    return false;
}

void SmartAttendanceSystem::resetTimeUntouched(){
    last_action = millis();
}

void SmartAttendanceSystem::validateAndSendId(){
    String id = display.getDisplayed();
    if (id.length() == ID_LENGTH){
        // check if ID already exists
        if(files.idExists(id)){
            display.displayWithDelay(ID_TAKEN);
            resetState();
        }
        else{
            files.addPendingUserEntry(id, uid_in_registration);
            Serial.println("Got " + id);
            display.displayWithDelay(SENDING_USER_INFO);
            callSync();
            resetState();
        }
    }
    else{
        display.displayWithDelay(ID_NOT_VALID, SHORT_MSG_DELAY);
        resetState();
    }
}

void SmartAttendanceSystem::resetState(){
    state = WAIT_FOR_CARD;
    display.clear();
    display.println(SWIPE_CARD);
}

void sync_thread(void* sync) {
    time_t start = millis();
    static_cast<Sync*>(sync)->sync();
    time_t finish = millis();
    Serial.println("Sync took " + String(finish - start) + " millis.");
    vTaskDelete(nullptr);
}