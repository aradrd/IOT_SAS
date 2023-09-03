#include <Wire.h>

#include <consts.h>
#include <Display.h>
#include <IOTKeypad.h>
#include <GoogleSheet.h>
#include <RFID.h>
#include <IOTFiles.h>
#include <IOTTime.h>
#include <IOTFiles.h>

// Glob inits.
IOTTime iot_time;
IOTFiles files(iot_time);
Display display1(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT);
Display display2(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT);
IOTKeypad keypad(display2, files);
GoogleSheet googleSheet(files);
RFID rfid(files);

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
  keypad.init();
  display1.init();
  display2.init();
  rfid.init();
  files.init();
  iot_time.init();

  // googleSheet.readDataFromGoogleSheet();
  // googleSheet.post_data();
}

void loop() {
  keypad.tick();
  String uid = rfid.tick();
  if (uid != "") {
    Serial.println("Found RFID: " + uid);
    if (uid == "A7 48 B3 4E ") {
      String entry = files.addAttendanceLogEntry(uid);
      googleSheet.addAttendanceLogEntry(entry);
    }
    else {
      display1.println("UID not recognized.\nHave a nice day.");
    }
  }
  delay(DELAY);
}
