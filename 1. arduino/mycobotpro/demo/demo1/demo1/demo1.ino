#include <MycobotBasic.h>
#include <ParameterList.h>
#include <SCServo.h>

MycobotBasic myCobot;
int k = 1;
SMSBL sm;

Coords pos_data;
Angles angles_data;

int power_pin = 2;
int open_pin = 5;
int out_state = 1;
int angles_init[4][6] = { {0.61, -22.41, -128.67, 58.88, 94.30, 1.31},
                          {48.86, -13.53, -77.69, -6.15, 96.67, -2.46},
                          {0.61, -13.53, -77.69, -6.15, 96.67, -2.46},
                          {90, -45, -85, 50, 0, -45}};

void setup() {
  myCobot.setup();
  M5.Lcd.setTextSize(3);
  myCobot.powerOn();
  pinMode(power_pin, OUTPUT);
  pinMode(open_pin, OUTPUT);
  xibeng(1);
  myCobot.setLEDRGB(255, 255, 255);
  Serial.begin(115200);
  delay(100);
  digitalWrite(power_pin, 1);
  init_pos();
}

void loop() {
  // put your main code here, to run repeatedly:
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
     if (M5.BtnA.wasReleased()) {
      reless();
      } 
    if (M5.BtnB.wasReleased()) {
      runing_angles();
      init_pos();
      }
    if (M5.BtnC.wasReleased()) {
//      runing_angles();  
      read_Angles();
    }
}

void init_pos(){
  Angles angles;
  for(int i = 0; i < 6; i++){
    angles[i] = angles_init[0][i];
  } 
  myCobot.writeAngles(angles, 20);
  delay(2000);
}

void reless()
{
  for(int i = 1; i<7; i++){
    myCobot.setServoData(i, 40, !k);
    delay(30);
  }
  k = !k;
}

void read_Angles()
{
  Angles data2;
  data2 = myCobot.getAngles();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data2[i]);
    Serial.print(", ");
  }
  Serial.println();
  angles_data = data2;
}

void read_pos()
{
  Coords data1;
  data1 = myCobot.getCoords();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data1[i]);
    Serial.print(", ");
  }
  Serial.println();
  pos_data = data1;
}

void down_pos()
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i<9; i++){
    myCobot.writeCoords(data1,20);
    data1[2] -= 10;
    delay(50);
  }
}

void up_pos()
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i<8; i++){
    myCobot.writeCoords(data1,20);
    data1[2] += 12;
    delay(50);
  }
}

void runing_angles()
{
  Angles angles;
  for(int j = 0; j<2; j++){
    for(int k = 0; k < 6; k++){
      angles[k] = angles_init[j][k];
    } 
    myCobot.writeAngles(angles,50);
    delay(2000);
    if(j == 0 or j == 2){
      down_pos();
      delay(2000);
      xibeng(0);
      up_pos();
      delay(3000);
    }
    else{
      xibeng(1);
      delay(2000);
    }
    
    Angles pos;
    for(int k = 0; k < 6; k++){
      pos[k] = angles_init[2][k];
      Serial.print(pos[k]);
    } 
    myCobot.writeAngles(pos,50);  
    delay(1000);
  }
}

void xibeng(int out_state){
  Serial.print(out_state);
  int state = out_state;
  switch (out_state)
  {
    Serial.print(state);
  case 0:
    digitalWrite(power_pin, 0);
    digitalWrite(open_pin, 0);
    break;

  case 1:
    digitalWrite(power_pin, 1);
    digitalWrite(open_pin, 1);
    break;
  
  default:
    break;
  }
}
