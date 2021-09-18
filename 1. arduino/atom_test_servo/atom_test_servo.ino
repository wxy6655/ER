#include "M5Atom.h"
#include <SCServo.h> 
#include <HardwareSerial.h>
HardwareSerial mycobotproatom(2);
SMSBL sm;
void setup() {
  // put your setup code here, to run once:
    M5.begin(true, false, true);
    delay(10);
    mycobotproatom.begin(1000000,SERIAL_8N1, 25, 21); 
    sm.pSerial = &mycobotproatom;
}

void loop() {
  // put your main code here, to run repeatedly:
  
    M5.update();
    delay(50);

    if (M5.Btn.wasPressed())
    {
        for(int i =1; i <7; i++)
      {
        sm.WritePosEx(i, 2048, 1000, 100);
        delay(1500);
      }
        for(int i =1; i <7; i++)
      {
        i = 2;
        sm.WritePosEx(i, 1050, 1000, 100);
        delay(1500);
        sm.WritePosEx(i, 3000, 1000, 100);
        delay(3500);
        sm.WritePosEx(i, 2000, 1000, 100);
        delay(1500);
      }
    }
    Serial.println();
    Serial.print("find motor id : ");
    for(int i =1; i <7; i++)
    {
      if(sm.Ping(i)>0){
        Serial.print(sm.Ping(i));
        delay(23);
        Serial.print(" ");
      }
    }
    delay(100); 
}
