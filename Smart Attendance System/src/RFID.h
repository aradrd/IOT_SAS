#ifndef IOT_RFID_H
#define IOT_RFID_H

#include <cassert>
#include <cstdint>

#include <SPI.h>
#include <MFRC522.h>

#include <consts.h>
#include <IOTExceptions.h>

class RFID {
public:
    RFID() : rfid(RFID_SS, RFID_RST) {}
    RFID(const RFID&) = default;
    RFID& operator=(const RFID&) = delete;
    ~RFID() = default;

    void init() {
        if (!Serial) {
            throw SerialUnintializedException();
        }
        SPI.begin();
        rfid.PCD_Init();
        is_initialized = true;
    }

    void tick() {
        if (!is_initialized) {
            throw ComponentUninitializedException("RFID");
        }

        if (!rfid.PICC_IsNewCardPresent()) {
            return;
        }

        // Select one of the cards
        if (!rfid.PICC_ReadCardSerial()) {
            Serial.println("Failed reading card serial.");
            return;
        }

        // Dump debug info about the card; PICC_HaltA() is automatically called
        rfid.PICC_DumpToSerial(&(rfid.uid));
    }

private:
    MFRC522 rfid;
    bool is_initialized = false;
};

#endif