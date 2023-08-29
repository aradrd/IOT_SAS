#include <Wire.h>

#include <consts.h>
#include <Display.h>
#include <IOTKeypad.h>
#include <GoogleSheet.h>
#include <RFID.h>

// Glob inits.
Display display1(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT);
Display display2(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT);
IOTKeypad keypad(display2);
GoogleSheet googleSheet;
RFID rfid;

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
  keypad.tick();
  String uid = rfid.tick();
  if (uid != "") {
    Serial.println("Found RFID: " + uid);
  }
  delay(DELAY);
}
