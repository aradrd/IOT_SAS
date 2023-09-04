#include <Wire.h>

#include <consts.h>
#include <Display.h>
#include <IOTKeypad.h>
#include <GoogleSheet.h>
#include <RFID.h>
#include <IOTFiles.h>
#include <IOTTime.h>
#include <IOTFiles.h>
#include <SmartAttendanceSystem.h>

SmartAttendanceSystem sas;

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

void init_wifi() {
    WiFi.mode(WIFI_STA);
    IPAddress dns(8,8,8,8);
    WiFi.begin("TechPublic2"); //TODO: Scan SSIDs.
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.dnsIP());
}

void setup() {
  // Init comms.
  init_serial();
  init_i2c();
  init_wifi();

  // Init componenets.
  sas.init();

  // googleSheet.readDataFromGoogleSheet();
  // googleSheet.post_data();
}

void loop() {
  sas.tick();
  delay(DELAY);
}
