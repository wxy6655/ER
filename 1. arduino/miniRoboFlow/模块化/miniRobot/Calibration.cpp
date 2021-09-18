#include "Calibration.h"

int calibrate_servo_no = 1;

void Calibration::bration(MycobotBasic &myCobot){
  M5.Lcd.setTextSize(2);
  // EEPROM.begin(EEPROM_SIZE);//new
  myCobot.setLEDRGB(255, 255, 255);
  Calibration::info();
  while (1)
  {
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
    if (M5.BtnA.wasPressed()) {
      myCobot.setLEDRGB(255, 0, 0);
      Calibration::init(myCobot);
      } 
    if (M5.BtnB.wasPressed()) {
        myCobot.setLEDRGB(0, 255, 0);
        Calibration::test(myCobot);
      }
    if (M5.BtnC.wasReleasefor(1000)) {
      myCobot.setLEDRGB(0, 0, 255);
      Calibration::reset(myCobot);
      break;
      }
  }
}

void Calibration::info()
{
  M5.Lcd.clear(BLACK);
  delay(50);
  M5.Lcd.setCursor(0,0);
  delay(50);
  if(lan == 2){
    M5.Lcd.drawString("按A键 - 设置舵机零位", 20, 40, 1);
    M5.Lcd.drawString("按B键 - 测试舵机", 20, 80, 1); 
    M5.Lcd.drawString("按C键 - 退出此程序",20, 120, 1);
  }
  if(lan == 1){
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.print("PressA - Calibrate Servo ");
    M5.Lcd.setCursor(0, 90);
   // M5.Lcd.print("PressB - Test Servos (long press to force testing)\n\n");
    M5.Lcd.print("PressB - Test Servos ");
    M5.Lcd.setCursor(0, 130);
    M5.Lcd.print("PressC - EXIT(1S)\n");
//    M5.Lcd.setCursor(0, 170);
//    M5.Lcd.print("(long press to return language selection)\n");    
  }
  M5.update(); 
} 

void Calibration::init(MycobotBasic &myCobot)
{
  M5.Lcd.clear(BLACK);
  delay(50);
  
  if (calibrate_servo_no>6) 
  {
    if(lan == 2){
      M5.Lcd.drawString("已经设置好所有舵机", 20, 20, 1);      
    }
    if(lan == 1){
      M5.Lcd.setCursor(0, 30);   
      M5.Lcd.print("Already Calibrate all \n");      
    }
    delay(500);
    Calibration::info();
    return;
  }
  
  myCobot.setServoCalibration(calibrate_servo_no);

  if(lan ==  2){
    M5.Lcd.drawString("已设置舵机", 20, 20, 1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.printf("%d",calibrate_servo_no);
    M5.Lcd.setTextSize(2);    
  }
  if(lan == 1){
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.print("Calibrating\nServo\n\n");
    M5.Lcd.setTextSize(4);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.print("%d",calibrate_servo_no);
    M5.Lcd.setTextSize(2); 
  }
  
  delay(100);

  myCobot.setEncoder(calibrate_servo_no, 2047);
  delay(400);

  calibrate_servo_no ++;
}

void Calibration::test(MycobotBasic &myCobot)
{
  M5.Lcd.clear(BLACK);
  delay(50);
  // move all servos
  if (calibrate_servo_no > 6)
  {
     for (int i = 1; i < 7; i ++)
    {
      if(lan == 2){
       M5.Lcd.drawString("已设置舵机零位 ", 20, 20, 1); 
      }
      if(lan == 1){
       M5.Lcd.setCursor(0, 30);
       M5.Lcd.printf("Move servo %d \n",i);  
      }

      myCobot.setEncoder(i, 1848);
      delay(2500);
      myCobot.setEncoder(i, 2248);
      delay(3000);
      myCobot.setEncoder(i, 2048);
      delay(2500);
    }
    Calibration::info();
    delay(2000);
  }
  else{
    if(lan == 2){
     M5.Lcd.drawString("请先设定关节零位", 20, 20, 1);     
    }
    if(lan == 1){
     M5.Lcd.setCursor(0, 30);
     M5.Lcd.print("Only move after all servo calibrated");
    }
    Calibration::info();
    delay(2000);
    return;
  }
}

void Calibration::reset(MycobotBasic &myCobot)
{
  M5.Lcd.clear(BLACK);
  delay(50);
  if(lan == 2){
   M5.Lcd.drawString("重新设置", 20, 20, 1);
  }
  if(lan == 1){
   M5.Lcd.setCursor(0, 30);
   M5.Lcd.print("Restart to calibrate");  
  }
  calibrate_servo_no = 0;
  //关闭扭力输出
  myCobot.setFreeMove();
  delay(1000);
  Calibration::info();
}
