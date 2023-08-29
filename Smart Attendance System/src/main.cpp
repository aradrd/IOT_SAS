#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#include <Display.h>
#include <IOTKeypad.h>
#include <GoogleSheet.h>

const uint8_t DISPLAY1_WIDTH = 128;
const uint8_t DISPLAY1_HEIGHT = 64;

const uint8_t DISPLAY2_WIDTH = 128;
const uint8_t DISPLAY2_HEIGHT = 32;

const uint8_t I2C_SDA_2 = 33;
const uint8_t I2C_SCL_2 = 32;

// RFID consts.
const uint8_t RFID_RST = 2;
const uint8_t RFID_SS = 5;

// WiFi credentials
//const char* ssid = "Sas project";
const char* ssid = "TechPublic";
const char* password = "12345678";

// Glob inits.
MFRC522 mfrc522(RFID_SS, RFID_RST);
Display display1(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT);
Display display2(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT);
IOTKeypad keypad(display2);
GoogleSheet googleSheet(ssid, password);

uint8_t i = 0;

void setup() {
  Serial.begin(9600);

  // Init I2C.
  Wire.begin();
  Wire.setClock(400000);
  Wire1.begin(I2C_SDA_2, I2C_SCL_2, 400000);

  display1.init();
  display2.init();

  keypad.init();

  // Init SPI and RFID.
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  googleSheet.init();
  googleSheet.readDataFromGoogleSheet();
  googleSheet.post_data();
}

void RFIDLoop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void loop() {
  i++;

  keypad.loop();
  RFIDLoop();
  delay(10);
}
