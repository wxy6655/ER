// SR09 修改I2C地址
#include <Wire.h>
#define ADDRESS  0xE8 >> 1    // default address of SR09 is 0xE8
#define NEWADDRESS    0xD0    //i2c address support:0xD0 0xD2 0xD4 0xD6 0xD8 0xDA 0xDC 0xDE 
//0xE0 0xE2 0xE4 0xE6 0xE8 0xEA 0xEC 0xEE 0xF8 0xFA 0xFC 0xFE

uint8_t old_address, new_address;

void setup()
{
  Serial.begin(9600);                     // start Serial transaction
  Wire.begin();                           // join the TWI as the master
  SR09_send_command(ADDRESS, 0x02, 0X71); // powered by USB
  delay(2000);                            // wait 2s
  Serial.print("Current address: ");
  old_address = i2cScanPrint();
  // Change the iic address
  SR09_send_command(ADDRESS, 0x02, 0x9A);
  delay(1);
  SR09_send_command(ADDRESS, 0x02, 0x92);
  delay(1);
  SR09_send_command(ADDRESS, 0x02, 0x9E);
  delay(1);
  SR09_send_command(ADDRESS, 0x02, NEWADDRESS);
  delay(100);
  Serial.print("New address: ");
  new_address = i2cScanPrint();

  if (old_address != new_address)
    Serial.println("Changed, Please power on again!");
  else
    Serial.println("Change error, Please check the current address again!");
}

void loop()
{
}

void SR09_send_command(byte address, byte reg, byte command) {  // send the command to SR09
  Wire.beginTransmission(address);    // start the transmission with SR09
  Wire.write(reg);                    // register 2
  Wire.write(command);                // send the command to the register 2
  Wire.endTransmission();             // end of transmission
}

uint8_t i2cScanPrint() {
  uint8_t error, address;

  for (address = 0x03; address < 0x78; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("0x");
      Serial.print(address, HEX);
      Serial.println(", ");
      return address;
    }
  }
}
