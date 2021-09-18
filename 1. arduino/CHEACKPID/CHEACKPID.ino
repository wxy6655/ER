#include <MycobotBasic.h>
#include <ParameterList.h>

MycobotBasic myCobot;
int baundrate = 115200;

void setup() {
  myCobot.setup();
  myCobot.powerOn();
  Serial.begin(9600);
  delay(200);
}

void loop() {

  M5.update();
  if (M5.BtnA.wasReleased()) {

   for(int i = 1; i <7 ; i++)
   {
     myCobot.setServoData(i, 24, 0);//ID
     delay(30);
     myCobot.setServoData(i, 26, 3);//ID
     delay(30);
     myCobot.setServoData(i, 27, 3);//ID
     delay(30);
     Serial.println("servo");
     Serial.print(i);
     Serial.println("CHACK PID OK");
   }
   

  } else if (M5.BtnB.wasReleased()) {
    for(int i =1; i <7; i++)
    { 
       Serial.print("servo: ");
       Serial.print(i);
       Serial.print(" P: ");
       Serial.print(myCobot.getServoData(i, 24));//P
       delay(50);
       Serial.print(" D: ");
       Serial.print(myCobot.getServoData(i, 26));//D
       delay(50);
       Serial.print(" I: ");
       Serial.print(myCobot .getServoData(i, 27));//I
       delay(50);
       Serial.println();
  }
}
}
