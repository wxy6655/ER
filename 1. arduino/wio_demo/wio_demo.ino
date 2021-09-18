#include <SoftwareSerial.h>
//实例化软串口
//SoftwareSerial Serial1(D2, D3); // RX, TX

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
  }

  Serial.println("Goodnight moon!");

  Serial1.begin(1000000);
  
  Serial1.println("Hello, world?");
}

void loop()
{
  while(Serial.available()>0) {Serial1.write(Serial.read()); }
  while(Serial1.available()>0) {Serial.write(Serial1.read());} 
}
