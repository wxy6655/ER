#include "connect.h"

SMSBL sm;

void Connect::info(){
  M5.Lcd.clear(BLACK);
  delay(50);
  M5.Lcd.setCursor(0,0);
  delay(50);
  
  M5.Lcd.println("Press A - Test Servos Connect");
  M5.Lcd.println("Press B - Read robot information ");
  M5.Lcd.println("Press C - EXIT(1S)");
} 

void Connect::test(MycobotBasic &myCobot){
    info();
    sm.pSerial = &Serial2;
    while (1)
    {
        // put your main code here, to run repeatedly:
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
   // M5.Lcd.clear(BLACK);
  
     if (M5.BtnA.wasReleased()) {
      Connect::testServo(myCobot);
      } 
    if (M5.BtnB.wasReleased()) {
      Connect::info();
      }
    if (M5.BtnC.wasReleasefor(1000)) {
      break;
    }
}
}

void Connect::testServo(MycobotBasic &myCobot){
    M5.Lcd.clear(BLACK);
    delay(50);
    M5.Lcd.println("connect test");
    int state = myCobot.isPoweredOn();
    if(state == 1){
    M5.Lcd.println("atom: ok");
    }else{
    M5.Lcd.println("atom: no");
    }
    for(int i = 1; i<7;i++){
        M5.Lcd.print("servo:");
        M5.Lcd.print(i);
        if(sm.FeedBack(i) != -1){
        M5.Lcd.println("ok");
        }else{
        M5.Lcd.println("no");
        }
        delay(50);
    }
    delay(3000);
    info();
}




/*
//开发中。。。。
void Connect::ReadConfig(){
    
}
*/