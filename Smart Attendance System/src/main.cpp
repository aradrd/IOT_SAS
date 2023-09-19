#include <Wire.h>

#include <consts.h>
#include <SmartAttendanceSystem.h>

SmartAttendanceSystem sas;

void init_i2c() {
  Wire.begin();
  Wire.setClock(I2C_CLOCK);
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
  sas.init();
}

void loop() {
  sas.tick();
  delay(DELAY);
}
