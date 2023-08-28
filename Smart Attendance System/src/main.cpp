#include <Wire.h>
#include <I2CKeyPad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <Display.h>

const uint8_t DISPLAY1_WIDTH = 128;
const uint8_t DISPLAY1_HEIGHT = 32;

const uint8_t DISPLAY2_WIDTH = 128;
const uint8_t DISPLAY2_HEIGHT = 16;


const uint8_t I2C_SDA_2 = 33;
const uint8_t I2C_SCL_2 = 32;

// RFID consts.
const uint8_t RFID_RST = 2;
const uint8_t RFID_SS = 5;

// Keypad consts (and non-const).
const uint8_t KEYPAD_ADDR = 0x20;
char keymap[17] = "147*2580369#ABCD";

// WiFi credentials
//const char* ssid = "Sas project";
const char* ssid = "TechPublic";
const char* password = "12345678";

String GOOGLE_SCRIPT_ID = "AKfycbwE-wRd4-k9RimSp_svSTjKdhQbHha_pTppacQrqA0_s8QRCHWLDjZIl7IhHYOTQRB3";

// Glob inits.
MFRC522 mfrc522(RFID_SS, RFID_RST);
I2CKeyPad keyPad(KEYPAD_ADDR);
Display display1(&Wire, DISPLAY1_WIDTH, DISPLAY1_HEIGHT);
Display display2(&Wire1, DISPLAY2_WIDTH, DISPLAY2_HEIGHT);

uint8_t i = 0;
bool keyStillPressed = false;

void readDataFromGoogleSheet(){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";
    Serial.println("Making a request");
    Serial.println(url);
    http.begin(url.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    String payload;
    if (httpCode > 0) { //Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      display1.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}

void post_data(){
  String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";

  Serial.println("POST data to spreadsheet:");
  Serial.println(urlFinal);
  HTTPClient http;
  http.begin(urlFinal.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  // Data to send with HTTP POST
  String httpRequestData = "{\"something\":\"elina\"}";           
  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
  Serial.print("HTTP Status Code: ");
  Serial.println(httpResponseCode);
  String payload = http.getString();
  Serial.println(payload);
}

void init_wifi() {
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.begin(ssid);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);

  // Init I2C.
  Wire.begin();
  Wire.setClock(400000);
  Wire1.begin(I2C_SDA_2, I2C_SCL_2, 400000);

  display1.init();
  display2.init();

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

  // Init Wifi
  // init_wifi();
  //readDataFromGoogleSheet();
  // post_data();
}

void keypadLoop() {
  Wire.requestFrom(KEYPAD_ADDR, (uint8_t) 1);
  if (!keyStillPressed && keyPad.isPressed()) {
    char ch = keyPad.getChar();     // note we want the translated char
    Serial.print(ch);
    Serial.println(" pressed.");
    display1.print(ch);
  }
  keyStillPressed = keyPad.isPressed();
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
  RFIDLoop();
  delay(10);
}
