//#include <M5Atom.h>

// HardwareSerial Serial1(1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial1.begin(115200, SERIAL_8N1, 25, 21);
  // Serial1.begin(115200);
  Serial2.begin(115200);
//  Serial1 = &Serial1;

//  Serial.setRxBufferSize(512);
//  Serial1.setRxBufferSize(512);
}
// void loop() {
//   // put your main code here, to run repeatedly:
//   while(Serial.available()>0) {Serial1.write(Serial.read()); }
//   while(Serial1.available()>0) {Serial.write(Serial1.read());} 
// }

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0) {Serial2.write(Serial.read()); }
  while(Serial2.available()>0) {Serial.write(Serial2.read());} 
}
