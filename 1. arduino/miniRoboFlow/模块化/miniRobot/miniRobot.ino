#include <MycobotBasic.h>
#include "TransPonder.h"
#include "MainControl.h"
#include "Calibration.h"
#include "connect.h"

MycobotBasic myCobot;
Transponder tp;
MainControl mc;
Calibration cb;
Connect ct;

#define App_number 3


#define GREY 0x5AEB
int state = 0;
// bool EXIT = false;
int y_pos[] = {70, 95, 120, 145};


void setup(){
    myCobot.setup();                                      
    myCobot.powerOn();                                    //启动机械臂

    M5.Lcd.setTextSize(2);                                //设置lcd的显示文字大小
    M5.Lcd.clear(BLACK);
    menu_choice();
    select();
}

void loop(){
    M5.update(); 
  if (M5.BtnA.wasPressed()) {
    state += 1;
    select();
    } 
  if (M5.BtnB.wasPressed()) {  
    state -= 1;
    select();
  }
  if (M5.BtnC.wasPressed()) {  
    program_selection(myCobot, state);
    M5.Lcd.clear(BLACK);
    menu_choice();
    select();
  } 
}

void program_selection(MycobotBasic &myCobot, int state){
  switch (state)
  {
  case 0:
    mc.Control(myCobot);
    break;
  case 1:
    cb.bration(myCobot);
    break;
  case 2:
    tp.ponder(myCobot);
    break;
  case 3:
    ct.test(myCobot);
    break;
  default:
    break;
  }
  menu_choice();
}

void menu_choice(){
  
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(20,10);
    M5.Lcd.print("miniRoboFlow"); 

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(250,30);
    M5.Lcd.setTextColor(GREY); 
    M5.Lcd.print("v1.0");   

    M5.Lcd.drawFastHLine(0,45,320,GREY);
    M5.Lcd.drawFastHLine(0,195,320,GREY);

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(30,70);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("MainControl"); 
    M5.Lcd.setCursor(30,95);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("Calibration"); 
    M5.Lcd.setCursor(30,120);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("Transponder"); 
    M5.Lcd.setCursor(30,145);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("Connect test"); 

    M5.Lcd.setCursor(35, 210);
    M5.Lcd.print("DOWN");
    M5.Lcd.setCursor(140, 210);
    M5.Lcd.print(" UP ");
    M5.Lcd.setCursor(235, 210);
    M5.Lcd.print(" OK ");

}

void select(){
  if (state < 0)
    {
      state = App_number;
  }if (state > App_number)
  {
    state = 0;
  }
  M5.Lcd.fillRect(0, 70, 26, 90, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,y_pos[state]);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print(" >");
}