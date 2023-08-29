#ifndef IOT_RFID_H
#define IOT_RFID_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>

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

    String tick() {
        if (!is_initialized) {
            throw ComponentUninitializedException("RFID");
        }

        if (!rfid.PICC_IsNewCardPresent()) {
            return "";
        }

        // Select one of the cards
        if (!rfid.PICC_ReadCardSerial()) {
            Serial.println("Failed reading card serial.");
            return "";
        }

        String uid = get_uid(&(rfid.uid));
        rfid.PICC_HaltA();
        return uid;
    }

private:
    MFRC522 rfid;
    bool is_initialized = false;

    String get_uid(const MFRC522::Uid* const uid_obj) {
        String uid = "";
        for (byte i = 0; i < uid_obj->size; i++) {
            uid += byteToHexString(uid_obj->uidByte[i]);
            uid += " ";
        }
        return uid;
    }

    String byteToHexString(const uint8_t byte) {
        std::ostringstream oss;
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        std::string uid_str = oss.str();
        std::transform(uid_str.begin(), uid_str.end(), uid_str.begin(), ::toupper);
        return uid_str.c_str();
    }
};

#endif