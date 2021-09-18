#include <MycobotBasic.h>
#include <ParameterList.h>

MycobotBasic myCobot;

bool Servo_state = 1;

Coords pos_init_1 = {135.10, -45.10, 90.10, -179.10, 0.10, -90.10};

Angles angles_0 = {0, 0, 0, 0, 0, 0};
Angles angles_1 = {-90.01, -45, -90.30, 45.09, 0.70, 90.63};
Angles angles_2 = {-0, -45, -90.30, 45.09, 0.70, 90.63};               
Coords data1;

void setup() {
  myCobot.setup();
  myCobot.powerOn();
  myCobot.setLEDRGB(0, 0, 255);
  delay(100);
  myCobot.writeAngles(angles_0,30);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    M5.update(); // need to call update()  
     if (M5.BtnA.wasReleased()) {
      reless();
      } 
    if (M5.BtnB.wasReleased()) { 
      read_Angles();
    }
    if (M5.BtnC.wasReleased()) {
      read_pos();
}
}

void reless()
{
  for(int i = 1; i<7; i++){
    myCobot.setServoData(i, 40, !Servo_state);
    delay(30);
  }
  Servo_state = !Servo_state;
}


Angles read_Angles()
{
  Angles data2;
  data2 = myCobot.getAngles();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data2[i]);
    Serial.print(", ");
  }
  Serial.println();
  return data2;
}

Coords read_pos()
{
  Coords data1;
  data1 = myCobot.getCoords();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data1[i]);
    Serial.print(", ");
  }
  Serial.println();
  return data1;
}

void move_coords(int dir, int steps)    //坐标移动控制，控制移动方向和步数，一步等于10mm
{
  if (steps > 0){
    for(int i = 0; i<steps; i++){
      myCobot.writeCoords(data1,30);
      for(int i=0; i<6; i++){
        Serial.print(data1[i]);
        Serial.print(", ");
      }
      Serial.println("");
      data1[dir] += 10;
      delay(100);
  }
  }else{
    steps = 0-steps;
    for(int i = 0; i<steps; i++){
    myCobot.writeCoords(data1,30);
    for(int i=0; i<6; i++){
      Serial.print(data1[i]);
      Serial.print(", ");
    }
    Serial.println("");
    data1[dir] -= 10;
    delay(100);
  }
  } 
}
