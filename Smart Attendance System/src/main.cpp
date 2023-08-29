#include <Wire.h>

#include <Display.h>
#include <IOTKeypad.h>
#include <GoogleSheet.h>
#include <RFID.h>

const uint16_t BAUD = 9600;
const uint16_t I2C_CLOCK = 40000;

const uint8_t DISPLAY1_WIDTH = 128;
const uint8_t DISPLAY1_HEIGHT = 64;

const uint8_t DISPLAY2_WIDTH = 128;
const uint8_t DISPLAY2_HEIGHT = 32;

const uint8_t I2C_SDA_2 = 33;
const uint8_t I2C_SCL_2 = 32;

// Glob inits.
Display display1(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT);
Display display2(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT);
IOTKeypad keypad(display2);
GoogleSheet googleSheet;
RFID rfid;

uint8_t i = 0;

void init_i2c() {
  Wire.begin();
  Wire.setClock(I2C_CLOCK);
  Wire1.begin(I2C_SDA_2, I2C_SCL_2, I2C_CLOCK);
}

void init_serial() {
  Serial.begin(BAUD);
  while(!Serial);
  Serial.println("Initialized Serial.");
}

void setup() {
  // Init comms.
  init_serial();
  init_i2c();

  // Init componenets.
  keypad.init();
  display1.init();
  display2.init();
  rfid.init();

  googleSheet.init();
  googleSheet.readDataFromGoogleSheet();
  googleSheet.post_data();
}

void loop() {
  i++;

  keypad.tick();
  rfid.tick();
  delay(10);
}
