#include <MycobotBasic.h>
#include <ParameterList.h>

MycobotBasic myCobot;    

void setup() {
  //打开通讯串口
  myCobot.setup();
  //链接atom
  myCobot.powerOn();
  delay(200);
 for(int i=1; i<6; i++){
  myCobot.focusServo(i);
  delay(50);
 }
  
}

void loop() {
  // put your main code here, to run repeatedly:
 Angles _data_1 = myCobot.getAngles();
 delay(200);
 Angles _data_2 = myCobot.getCoords();
 delay(200);
 Serial.print("Angles: ");
 for(auto _data : _data_1){
  Serial.print(_data);
  Serial.print(",  ");
 }
  Serial.print("Coords: ");
 for(auto _data : _data_2){
  Serial.print(_data);
  Serial.print(",  ");
 }
 Serial.println(" ");
}
