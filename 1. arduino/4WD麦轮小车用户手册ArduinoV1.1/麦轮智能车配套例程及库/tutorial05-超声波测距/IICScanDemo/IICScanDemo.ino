/*
   IIC Scan address

   www.yfrobot.com.cn
*/

#include <Wire.h>

void setup() {
  Wire.begin();

  Serial.begin(9600);
  Serial.println("I2C Scanner");
}

void loop() {
  uint8_t error, address, line = 1;
  int nDevices = 0;

  Serial.println("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  Serial.print  ("00:         ");

  // https://learn.adafruit.com/i2c-addresses/the-list
  for (address = 0x03; address < 0x78; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      printHex(address);
      nDevices++;
    }
    else if (error == 4)
      Serial.print(" ER");
    else
      Serial.print(" --");

    if ((address + 1) % 16 == 0) {
      Serial.println();
      Serial.print(line);
      Serial.print("0:");
      line++;
    }

  }
  if (nDevices == 0)
    Serial.println("\nNo I2C devices found\n");
  else {
    Serial.print("\nFound ");
    Serial.print(nDevices);
    Serial.println(" devices\n");
  }

  delay(5000);           // wait 5 seconds for next scan
}

void printHex(uint8_t address) {
  Serial.print(" ");
  if (address < 16)
    Serial.print("0");
  Serial.print(address, HEX);
}
