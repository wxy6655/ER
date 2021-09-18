// SR09 IIC 模式下测距

#include <Wire.h>
#define ADDRESS  0xD4>>1   // default address of SR09 is 0xE8
#define CMDREG   0x02    //the command register is 0x02
#define CMDRD    0xBC    //read the distance of SR09,less 600cm.

void setup()
{
  Serial.begin(9600);                       // start Serial transaction
  Wire.begin();                             // join the TWI as the master
  SR09_send_command(ADDRESS,CMDREG,0X71);   // powered by USB
  delay(2000);                              // wait 2s
  Serial.println("Start...");
}

void loop()
{ 
  word xx =  SR09_read_data(ADDRESS,CMDREG,CMDRD);  //read the distance 
  Serial.print(xx);               //print the distance by serial
  Serial.println("mm");
  delay(200);
}

// 写指令
void SR09_send_command(byte address,byte reg,byte command) {    // send the command to SR09
  Wire.beginTransmission(address);    // start the transmission with SR09
  Wire.write(reg);                    // register 2
  Wire.write(command);                // send the command to the register 2
  Wire.endTransmission();             // end of transmission
}

// 读数据
word SR09_read_data(byte address,byte reg,byte command) { // sending the command and read the data in the register
  word temp_reading;
  // step 1: instruct sensor to read echoes
  SR09_send_command(address,reg,command);             // send the command
  // step 2: wait for readings to happen
  delay(88);                          // wait at least 87 milliseconds
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(address);    // start to transmit with SR09
  Wire.write(0x02);             // register 2 is the gate of receiving the data
  Wire.endTransmission();             // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(uint8_t(address), uint8_t(2));       // request the data in the 2nd and third register of SR09
  // step 5: receive reading from sensor
  while ( Wire.available() < 2) {}    // wait the register available
  temp_reading = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
  temp_reading |= Wire.read();        // read the register 3 to lower byte
  return temp_reading;                // return as a 16bit data
}
