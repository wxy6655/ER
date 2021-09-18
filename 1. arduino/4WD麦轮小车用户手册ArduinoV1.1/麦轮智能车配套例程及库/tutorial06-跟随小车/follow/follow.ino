/*
  Ultrasonic follow - 超声波跟随小车
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#include <Wire.h>

#define ADDRESS  0xD4>>1   // default address of SR09 is 0xE8
#define CMDREG   0x02    //the command register is 0x02
#define CMDRD    0xBC    //read the distance of SR09,less 600cm.

#define M1PinDir 4
#define M1PinSpeed 3
#define M2PinDir 7
#define M2PinSpeed 5
#define M3PinDir 8
#define M3PinSpeed 6
#define M4PinDir 12
#define M4PinSpeed 11

unsigned int dis_mm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(M1PinDir, OUTPUT);
  pinMode(M1PinSpeed, OUTPUT);
  pinMode(M2PinDir, OUTPUT);
  pinMode(M2PinSpeed, OUTPUT);
  pinMode(M3PinDir, OUTPUT);
  pinMode(M3PinSpeed, OUTPUT);
  pinMode(M4PinDir, OUTPUT);
  pinMode(M4PinSpeed, OUTPUT);
  Wire.begin();                             // join the TWI as the master
  SR09_send_command(ADDRESS, CMDREG, 0X71); // powered by USB
  delay(2000);                              // wait 2s
  Serial.println("Start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  dis_mm = UltrasonicRanging();
  Serial.print(dis_mm);
  Serial.println("mm");
  delay(80);
  if (dis_mm >= 110) {
    setMotor(100, 100, 100, 100);    //前进
  } else {
    setMotor(0, 0, 0, 0);
  }
}

/*
  驱动电机函数
  参数：m1Speed - 1电机速度 m2Speed - 2电机速度
       m3Speed - 3电机速度 m4Speed - 4电机速度 （取值范围：-255 ~ 255）
*/
void setMotor(int m1Speed, int m2Speed, int m3Speed, int m4Speed) {
  if (m1Speed > 0)
    digitalWrite(M1PinDir, LOW);
  else if (m1Speed < 0)
    digitalWrite(M1PinDir, HIGH);
  analogWrite(M1PinSpeed, abs(m1Speed));

  if (m2Speed > 0)
    digitalWrite(M2PinDir, HIGH);
  else if (m2Speed < 0)
    digitalWrite(M2PinDir, LOW);
  analogWrite(M2PinSpeed, abs(m2Speed));

  if (m3Speed > 0)
    digitalWrite(M3PinDir, LOW);
  else if (m3Speed < 0)
    digitalWrite(M3PinDir, HIGH);
  analogWrite(M3PinSpeed, abs(m3Speed));

  if (m4Speed > 0)
    digitalWrite(M4PinDir, HIGH);
  else if (m4Speed < 0)
    digitalWrite(M4PinDir, LOW);
  analogWrite(M4PinSpeed, abs(m4Speed));
}

int UltrasonicRanging() {
  return SR09_read_data(ADDRESS, CMDREG, CMDRD); //read the distance ;
}

// 写指令
void SR09_send_command(byte address, byte reg, byte command) {  // send the command to SR09
  Wire.beginTransmission(address);    // start the transmission with SR09
  Wire.write(reg);                    // register 2
  Wire.write(command);                // send the command to the register 2
  Wire.endTransmission();             // end of transmission
}

// 读数据
word SR09_read_data(byte address, byte reg, byte command) { // sending the command and read the data in the register
  word temp_reading;
  // step 1: instruct sensor to read echoes
  SR09_send_command(address, reg, command);           // send the command
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
