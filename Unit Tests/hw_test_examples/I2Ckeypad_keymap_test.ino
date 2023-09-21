//
//    FILE: I2Ckeypad_keymap.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo key mapping
//     URL: https://github.com/RobTillaart/I2CKeyPad
//
// PCF8574
//    pin p0-p3 rows
//    pin p4-p7 columns
// 4x4 or smaller keypad.


#include "Wire.h"
#include "I2CKeyPad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;

I2CKeyPad keyPad(KEYPAD_ADDRESS);

char keymap[17] = "DCBA#9630852*741";  // N = NoKey, F = Fail


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    while (1);
  }

  keyPad.loadKeyMap(keymap);
  Serial.println(keyPad.getKeyPadMode());
}


void loop()
{
  // read row and col
  // Wire.beginTransmission(KEYPAD_ADDRESS);
  // //Wire.write(0xFF);
  // if (Wire.endTransmission() != 0)
  // {
  //   Serial.print("error in Transmission");
  // }
  Wire.requestFrom(KEYPAD_ADDRESS, (uint8_t)1);
  Serial.println(Wire.read());

/*
  // read col
  Wire.beginTransmission(KEYPAD_ADDRESS);
  Wire.write(0x0F);
  if (Wire.endTransmission() != 0)
  {
    Serial.print("error in Transmission");
  }
  Wire.requestFrom(KEYPAD_ADDRESS, (uint8_t)1);
  Serial.println(Wire.read());
  */

  if (keyPad.isPressed())
  {
    char ch = keyPad.getChar();     // note we want the translated char
    int key = keyPad.getLastKey();
    Serial.print(key);
    Serial.print(" \t");
    Serial.println(ch);
  }

  delay(100);
}


// -- END OF FILE --

