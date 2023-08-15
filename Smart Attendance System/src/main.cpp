#include <Wire.h>
#include <I2CKeyPad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen consts.
const uint8_t DISPLAY_WIDTH = 128;
const uint8_t DISPLAY_HEIGHT = 32;
const uint8_t DISPLAY_RST = -1; // -1 means shared (whatever that means).
const uint8_t DISPLAY_ADDR = 0x3C;
const uint8_t DISPLAY_TEXT_SIZE = 1;
const uint8_t DISPLAY_TEXT_COLOR = WHITE;

// RFID consts.
const uint8_t RFID_RST = 2;
const uint8_t RFID_SS = 5;

// Keypad consts (and non-const).
const uint8_t KEYPAD_ADDR = 0x20;
char keymap[17] = "DCBA#9630852*741";

// Glob inits.
MFRC522 mfrc522(RFID_SS, RFID_RST);
I2CKeyPad keyPad(KEYPAD_ADDR);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, DISPLAY_RST);

uint8_t i = 0;
bool keyStillPressed = false;

void setup() {
  Serial.begin(9600);

  // Init I2C.
  Wire.begin();
  Wire.setClock(400000);

  // Init keypad.
  if (keyPad.begin() == false) {
    Serial.println("Can't communicate to keypad.");
  }
  else {
    keyPad.loadKeyMap(keymap);
    Serial.println(keyPad.getKeyPadMode());
  }

  // Init SPI and RFID.
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  // Init display.
  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  else {
    display.clearDisplay();
    display.setTextSize(DISPLAY_TEXT_SIZE);
    display.setTextColor(DISPLAY_TEXT_COLOR);
    display.setCursor(0, 0);
    display.println("Please Enter ID:");
    display.display();
  }
}

void keypadLoop() {
  Wire.requestFrom(KEYPAD_ADDR, (uint8_t) 1);
  if (!keyStillPressed && keyPad.isPressed()) {
    char ch = keyPad.getChar();     // note we want the translated char
    Serial.print(ch);
    Serial.println(" pressed.");
    display.print(ch);
    display.display();
  }
  keyStillPressed = keyPad.isPressed();
}

void displayLoop() {
  display.invertDisplay(i % 20 >= 10);
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

  keypadLoop();
  // displayLoop();
  RFIDLoop();

  delay(10);
}
