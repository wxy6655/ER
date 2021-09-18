#include <MycobotBasic.h>
// #include <MycobotLanguage.h>

MycobotBasic myCobot;
// MycobotLanguage mcLan;

typedef  unsigned char u8;

int lan = 1;
int calibrate_servo_no = 1;


byte IO_TIME_OUT = 25;
int IO_REC_WRONG = -1;
byte HEADER = 0xfe;
byte FOOTER = 0xfa;

typedef struct{
  int joint_angle[6];
} joint_angles_enc;

int data_len_max = 1000;
joint_angles_enc jae[1000];

byte control_pin = 26;   // PIN 26 high -> loop play data from Flash

int rec_data_len = 0;
byte mycobot_mode = 0; 

int error_display_time = 3000;

int state = 0;

int y_pos[] = {70, 95, 120, 145};

#define GREY 0x5AEB
int disp_mode = 0;

bool EXIT = false;

void setup() {
  // put your setup code here, to run once:
  myCobot.setup();
  myCobot.powerOn(); 
  // EEPROM.begin(EEPROM_SIZE);
  // EEPROM.write(Lan_Add, 0);
  // EEPROM.commit();
  M5.Lcd.setTextSize(3);
  Serial.begin(115200);
  Serial2.begin(115200);
  // lan = mcLan.language(); 
  M5.Lcd.clear(BLACK);
  menu_choice();
  select();
}

void(* resetFunc) (void) = 0;

void loop() {
  // put your main code here, to run repeatedly:
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
    program_selection(state);
    M5.Lcd.clear(BLACK);
    menu_choice();
    select();
  } 
}

void program_selection(int state){
  switch (state)
  {
  case 0:
    MainControl();
    break;
  case 1:
    Calibration();
    break;
  case 2:
    Transponder();
    break;
  case 3:

    break;
  default:
    break;
  }
  EXIT = false;
  menu_choice();
}

void updateUI(int dis_mode)
{
  M5.Lcd.clear(BLACK);
  int buttom_y = 210; 
  switch (dis_mode)
  {
    case 1:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("miniRoboFlow"); 

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(250,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("v1.0");   

      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,70);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("Load Program"); 
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(20,95);
      M5.Lcd.print("prgoram_test_1_here.elep"); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Run");

      M5.Lcd.setCursor(120,buttom_y);
      M5.Lcd.print("Release");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Setup");

      
      break;
    }
    case 2:
    {
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("miniRoboFlow"); 

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(250,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("v1.0");   

      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,70);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("Load Program"); 
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(20,95);
      M5.Lcd.print("prgoram_test_1_here.elep"); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Run");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Focus");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Setup");

      break;
    }
    case 3:
    {
      M5.Lcd.drawFastHLine(0,60,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("Running Program"); 
      M5.Lcd.setTextSize(2); 
      M5.Lcd.setTextColor(GREY);
      M5.Lcd.setCursor(20,40);
      M5.Lcd.print("- 875 h, 32 m, 21 s");

      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,75);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("prgoram_test_1.elep"); 
      

        
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.setCursor(0,115);
      M5.Lcd.print(">> moveToCoord(111.01)"); 
      M5.Lcd.setTextColor(YELLOW);
      M5.Lcd.setCursor(10,150);
      M5.Lcd.print("> moveToCoord(33121.01))"); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Run");

      M5.Lcd.setCursor(120,buttom_y);
      M5.Lcd.print("Release");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Setup");
      
      break;
    }
    case 4:
    {
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("Setups"); 
      
      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(30,60);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("1 Test Servos"); 
      
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(10,90);
      M5.Lcd.print("> 2 Monitor USB"); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(30,120);
      M5.Lcd.print("3 Monitor IOs");

      M5.Lcd.setCursor(30,150);
      M5.Lcd.print("4 Monitor Ethernet");


      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Down");

      M5.Lcd.setCursor(150,buttom_y);
      M5.Lcd.print("Go");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");


      break;
    }
    case 5:
    {
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("Setups"); 

     M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(140,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("Test Servos"); 
      
      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(30,60);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("OK: Servo 1 2 3 5"); 
      
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(17,90);
      M5.Lcd.print("NOK: Servo 4 6"); 


      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Focus");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Release");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");
      
      break;
    }
    case 6:
    {
       M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("Setups"); 

     M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(140,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("Monitor USB"); 
      
      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,60);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("Receing Data"); 
      
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(20,90);
      M5.Lcd.print("fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 "); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Clear");


      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");
      
      break;
    }
    case 7:
    {
       M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("Setups"); 

     M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(140,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("Monitor IO"); 
      
      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,60);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("Inputs 1~6"); 
      
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(20,90);
      M5.Lcd.print("0 | 1 | 0 | 0 | 0 | 1"); 

       M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,120);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("Inputs 7~12"); 
      
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setCursor(20,150);
      M5.Lcd.print("0 | 1 | 0 | 0 | 0 | 1"); 


      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Update");

  

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");
      
      break;
    }
    case 8:
    {
       M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,10);
      M5.Lcd.print("Setups"); 

     M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(140,18);
      M5.Lcd.setTextColor(GREY); 
      M5.Lcd.print("MonitorEthernet"); 
      
      M5.Lcd.drawFastHLine(0,45,320,GREY);
      M5.Lcd.drawFastHLine(0,195,320,GREY);
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20,60);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("Receing Data"); 
      
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(20,90);
      M5.Lcd.print("fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 fa fa 04 24 43 02 "); 

      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Clear");


      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");

      
      break;
    }
    case 9:
    {
      ;
    }
    
  break;
  }
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
}

void select(){
  if (state < 0)
    {
      state = 3;
  }if (state > 3)
  {
    state = 0;
  }
  M5.Lcd.fillRect(0, 70, 26, 90, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,y_pos[state]);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print(" >");
}

void Calibration(){
  M5.Lcd.setTextSize(2);
  // EEPROM.begin(EEPROM_SIZE);//new
  myCobot.setLEDRGB(255, 255, 255);
  Calibration_info();
  while (1)
  {
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
    if (M5.BtnA.wasPressed()) {
      myCobot.setLEDRGB(255, 0, 0);
      Calibration_init();
      } 
    if (M5.BtnB.wasPressed()) {
        myCobot.setLEDRGB(0, 255, 0);
        Calibration_test();
      }
    if (M5.BtnC.wasReleasefor(2000)) {
      myCobot.setLEDRGB(0, 0, 255);
      Calibration_reset();
      break;
      }
  }
}

void Calibration_info()
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
    M5.Lcd.print("PressC - EXIT\n");
//    M5.Lcd.setCursor(0, 170);
//    M5.Lcd.print("(long press to return language selection)\n");    
  }
  M5.update(); 
} 

void Calibration_init()
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
    Calibration_info();
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
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.printf("%d",calibrate_servo_no);
    M5.Lcd.setTextSize(2); 
  }
  
  delay(100);

  myCobot.setEncoder(calibrate_servo_no, 2047);
  delay(400);

  calibrate_servo_no ++;
}

void Calibration_test()
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
    Calibration_info();
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
    Calibration_info();
    delay(2000);
    return;
  }
}

void Calibration_reset()
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
  Calibration_info();
}

void MainControl(){
  EXIT = false;
  pinMode(control_pin, INPUT);
  myCobot.setLEDRGB(0,255,0);
  maincontrol_updateMode(mycobot_mode);
  maincontrol_displayInfo(0);
  while(1){
    M5.update(); 
    byte btn_pressed = 0;   
    if (M5.BtnA.wasPressed()) {
        btn_pressed = 1;
        maincontrol_updateMode(btn_pressed);
      } 
    if(M5.BtnB.wasPressed()){
      btn_pressed = 2;
      maincontrol_updateMode(btn_pressed);
      }
    if (M5.BtnC.wasPressed()) {
        btn_pressed = 3;
        maincontrol_updateMode(btn_pressed);
      }
    if(EXIT){break;}  
    maincontrol_IO(); 
  }
}

void maincontrol_updateMode(byte btn_pressed)
{
  if (mycobot_mode == 0)
  {
    switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 1;
        break;
      case 2:
        mycobot_mode = 2;
        break;
      case 3:
        myCobot.setFreeMove();
        mycobot_mode = 0;
        EXIT = true;
        break;
    } 
    maincontrol_displayInfo(mycobot_mode);
  }
  else if(mycobot_mode == 1)
  {    
     switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 11;
        maincontrol_displayInfo(mycobot_mode);
        maincontrol_play(); // play loop from ram

        break;
      case 2:
        mycobot_mode = 12; 
        maincontrol_playFromFlash(); // play loop from flash
        
        break;
      case 3:
        mycobot_mode = 0;   // get back
        maincontrol_displayInfo(mycobot_mode);
        break;
    }
    
  }

  else if(mycobot_mode == 2)
  {
    
    switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 21;
        maincontrol_displayInfo(mycobot_mode);
        maincontrol_record();

        // finish record
        maincontrol_displayInfo(33);
        delay(2000);

        // recover to original
        mycobot_mode = 0;
        maincontrol_displayInfo(mycobot_mode);
        break;
        
      case 2:
        mycobot_mode = 22;  // record into ram as well
        maincontrol_displayInfo(mycobot_mode);
        maincontrol_recordIntoFlash();
        break;
      case 3:
        mycobot_mode = 0;
        maincontrol_displayInfo(mycobot_mode);
        break;
    }
    
  }

  else if((mycobot_mode == 11)||(mycobot_mode == 12))
  {
     switch (btn_pressed)
    {
      case 1:
        Serial.println("Continue Play");
        
        break;
      case 2:
        Serial.println("Pause");
  
        break;
      case 3:
        Serial.println("Stop");
        mycobot_mode = 0;
        break;
    }
  }

  else if((mycobot_mode == 21)||(mycobot_mode == 22))
  {
    switch (btn_pressed)
    {
      case 2:
        Serial.println("Save and Stop");
        break;
      case 3:
        Serial.println("stop record");
        break;
      
    }
     
    mycobot_mode = 0;
    maincontrol_displayInfo(mycobot_mode);
  }
  
}

void maincontrol_displayInfo(byte mc_mode)
{
  M5.Lcd.clear(BLACK);
  delay(50);
  M5.Lcd.setTextSize(2);
  int buttom_y = 190;
  int buttom_1y = 210;
  int buttom_2y = 210;
  
 switch (mc_mode) { 
    case 0:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString("  MyCobot-拖动示教", 20, 40, 1);
          M5.Lcd.drawString("播放", 60, buttom_y, 1);
          M5.Lcd.drawString("录制", 160, buttom_y, 1);
          M5.Lcd.drawString("退出", 260, buttom_y, 1);
          M5.update();
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(20, 40);
          M5.Lcd.print("MyCobot - Menu");
          M5.Lcd.setCursor(30, 210);
          M5.Lcd.print("Play");
          M5.Lcd.setCursor(120, 210);
          M5.Lcd.print("Record");
          M5.Lcd.setCursor(230, 210);
          M5.Lcd.print("EXIT");
          M5.update();
        }
        break;
      }

    case 1:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 请选择示教路径的播放位置", 10, 40, 1);
          M5.Lcd.drawString("缓存", 60, buttom_y, 1);
          M5.Lcd.drawString("储存卡", 160, buttom_y, 1);
          M5.Lcd.drawString("返回", 260, buttom_y, 1);
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(20, 40);
          M5.Lcd.print("Playing for Ram/Flash?");
          M5.Lcd.setCursor(30, buttom_1y);
          M5.Lcd.print("Ram");
          M5.Lcd.setCursor(136, buttom_1y);
          M5.Lcd.print("Flash");
          M5.Lcd.setCursor(240, buttom_1y);
          M5.Lcd.print("Back");
        }
        break;
      }
    case 2:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 请选择示教视频储存路径", 20, 40, 1);
          M5.Lcd.drawString("缓存", 60, buttom_y, 1);
          M5.Lcd.drawString("储存卡", 160, buttom_y, 1);
          M5.Lcd.drawString("返回", 260, buttom_y, 1);         
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(20, 40);
          M5.Lcd.print("Recording to Ram/Flash?");
          M5.Lcd.setCursor(30, buttom_1y);
          M5.Lcd.print("Ram");
          M5.Lcd.setCursor(136, buttom_1y);
          M5.Lcd.print("Flash");
          M5.Lcd.setCursor(240, buttom_1y);
          M5.Lcd.print("Back");
        }
        break;
      }
    case 11:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 正在执行缓存中的路径", 20, 40, 1);
          M5.Lcd.drawString(" 播放中...", 20, 70, 1);
          M5.Lcd.drawString("开始", 60, buttom_y, 1);
          M5.Lcd.drawString("暂停", 160, buttom_y, 1);
          M5.Lcd.drawString("结束", 260, buttom_y, 1);
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(20, 40);
          M5.Lcd.print("Play from Ram\n  Playing...");
          M5.Lcd.setCursor(40, buttom_2y);
          M5.Lcd.print("Play");
          M5.Lcd.setCursor(130, buttom_2y);
          M5.Lcd.print("Pause");
          M5.Lcd.setCursor(230, buttom_2y);
          M5.Lcd.print("Stop");
        }
        break;
      }
    case 12:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 正在执行储存卡中的路径", 20, 40, 1);
          M5.Lcd.drawString(" 播放中...", 20, 70, 1);
          M5.Lcd.drawString("开始", 60, buttom_y, 1);
          M5.Lcd.drawString("暂停", 160, buttom_y, 1);
          M5.Lcd.drawString("结束", 260, buttom_y, 1);
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(0, 40);
          M5.Lcd.print(" Play from Flash/nPlaying");
          M5.Lcd.setCursor(40, buttom_2y);
          M5.Lcd.print("Play");
          M5.Lcd.setCursor(130, buttom_2y);
          M5.Lcd.print("Pause");
          M5.Lcd.setCursor(230, buttom_2y);
          M5.Lcd.print("Stop");
        }
        break;
      }

    case 21:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 录制并存入缓存", 20, 40, 1);
          M5.Lcd.drawString(" 录制中...", 20, 70, 1);
          M5.Lcd.drawString(" 停止录制并保存", 5, buttom_y, 1);
        }
        else if (lan == 1)
        {
          M5.Lcd.setCursor(0, 40);
          M5.Lcd.print("Record into Ram\nRecording...");
          M5.Lcd.setCursor(5, buttom_1y);
          M5.Lcd.print("Stop Recording and Save");
        }
        break;
      }
    case 22:
      {
        M5.Lcd.fillScreen(0);
        if (lan == 2)
        {
          M5.Lcd.drawString(" 录制并存入储存卡", 20, 40, 1);
          M5.Lcd.drawString(" 录制中...", 20, 70, 1);
          M5.Lcd.drawString(" 停止录制并保存", 5, buttom_y, 1);
        }
        if (lan == 1)
        {
          M5.Lcd.setCursor(0, 40);
          M5.Lcd.print("Record into Flash\nRecording...");
          M5.Lcd.setCursor(5, buttom_y);
          M5.Lcd.print("Stop Recording and Save");
        }

        break;
      }

    case 32: // Stop recording
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString(" 暂停中", 20, 40, 1);
        M5.Lcd.drawString("开始", 60, buttom_y, 1);
        M5.Lcd.drawString("暂停", 160, buttom_y, 1);
        M5.Lcd.drawString("结束", 260, buttom_y, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print(" Puase Now"); 
        M5.Lcd.setCursor(40,buttom_2y);
        M5.Lcd.print("Play");
        M5.Lcd.setCursor(130,buttom_2y);
        M5.Lcd.print("Pause"); 
        M5.Lcd.setCursor(230,buttom_2y);
        M5.Lcd.print("Stop");
          }
        break;
      }

    case 33: // Stop recording
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString(" 保存录制", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("  Saved Recording"); 
          }
        break;
      }

    case 41: // Play from Flash
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString("从储存卡中获取数据", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("Getting data from Flash"); 
          }
        break;
      }

    case 42: // Record from Flash
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString("将数据保存到储存卡", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("Saving Data into Flash");
          }
        break;
      }

    case 51: // loop play from sram
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString("IO口状态", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("IO Pin Active!");
          }
        break;
      }

    case 52: // loop play from sram
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString("数据不足，无法播放", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("Data too short, not playing");
          }
        break;
      }

    case 53: // loop play from sram
      {
        M5.Lcd.fillScreen(0);
        if(lan == 2){
        M5.Lcd.drawString("无数据", 20, 40, 1);
        }
        else if(lan == 1){
        M5.Lcd.setCursor(0,40);
        M5.Lcd.print("Empty data!");
          }
        break;
      }
     default: ;break;   
  }
}

void maincontrol_record()  // is stop
{
  myCobot.setLEDRGB(255,255,0);
  // record mode : 1- record to ram;  2- record to flash
  rec_data_len = 0;
  for (int data_index = 0; data_index <data_len_max ; data_index ++)
  {
    M5.update(); 
    for (int i = 0 ; i < 6; i ++)
    {
      jae[data_index].joint_angle[i] = myCobot.getEncoder(i);
      Serial.print(jae[data_index].joint_angle[i]);
      Serial.print(" ");
      delay(REC_TIME_DELAY - SEND_DATA_GAP);
      delay(10);
    }
    Serial.println(" ");
    rec_data_len++;
    if (M5.BtnA.wasPressed()||M5.BtnB.wasPressed()||M5.BtnC.wasPressed()) break;
  } 
}

void maincontrol_play()  // is stop  is pause
{
  myCobot.setLEDRGB(0,255,0);
 
  bool is_stop = 0;
  bool is_pause = 0;

  while(1)
  {
    Serial.print("123");
    // play once
    for (int index = 0 ; index < rec_data_len; index++)
    {
      M5.update(); 
      Angles encoders;
      for(int i = 0; i<6; i++){
        encoders[i] = jae[index].joint_angle[i];
      }
//      int encoder_7 = jae[index].joint_angle[6];
      myCobot.setEncoders(encoders, 100);
      // delay(50);
      // myCobot.setEncoder(7, encoder_7);
      // check pause button
      if (M5.BtnB.wasPressed())
      {
        maincontrol_displayInfo(32);
        while(1)
        {
          M5.update(); 
          if (M5.BtnA.wasPressed())
          {
            maincontrol_displayInfo(11);
            break;
          }
          if (M5.BtnC.wasPressed()) 
          {
            is_stop = 1;
            break;
          }
        }
      }

      // check stop button
      if (M5.BtnC.wasPressed())
      {
        M5.update(); 
        is_stop = 1;
        break;
      }

      // check stop button
      if(is_stop == 1) break;
      delay(WRITE_TIME_GAP*6); 
    }

    // stop button will also end loop
    if(is_stop == 1){
      break;
    }

    // data too short
    if(rec_data_len < 10) break;

    // quick loop
    if (M5.BtnC.wasPressed())break;
  }

  // recover to play page
  M5.update(); 
  mycobot_mode = 1;
  maincontrol_displayInfo(mycobot_mode);
}

void maincontrol_playFromFlash()
{
    M5.update(); 
    maincontrol_displayInfo(41);

    // initialization first 
     if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    Serial.printf("Reading file: %s\r\n", FILENAME);
    File file = SPIFFS.open(FILENAME);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    String this_line = "";
    int index = 0;

    int t1= millis();
    
    while(file.available()){
      char this_char = char(file.read());
      this_line += this_char;
      if (this_char == '\n')
      {
        MycobotSaver::saver_angles_enc sae_this;
        sae_this = myCobot.saver.processStringIntoInts(this_line);

        for(int jn = 0; jn<6; jn++)
        {
          jae[index].joint_angle[jn] = sae_this.joint_angle[jn];
        }
        index ++;
        this_line = "";
      }
      if(index>data_len_max) break;
    }

    // update the len
    rec_data_len = index - 1;

    // play from flash
    M5.update(); 
    maincontrol_displayInfo(mycobot_mode);
    maincontrol_play();
}

void maincontrol_recordIntoFlash()
{
    // recording data
    maincontrol_record();
    M5.update(); 
    // show saving to recording
    maincontrol_displayInfo(33);

    // initialize flash
     if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // list exisiting files 
    myCobot.saver.listDir(SPIFFS, "/", 0);
    
    // clean exsiting file
    myCobot.saver.writeFile(SPIFFS, FILENAME, " ");

    // check time
    File file = SPIFFS.open(FILENAME, FILE_APPEND);

    for (int data_index = 0; data_index < rec_data_len; data_index ++)
    {
      String data_output = "";
      for (int l = 0; l < 6; l ++)
      {
        data_output +=  jae[data_index].joint_angle[l];
        data_output += ",";
      }
      data_output += "\n";
      file.print(data_output);
    }
    
   // recover to original
   mycobot_mode = 0;
   maincontrol_displayInfo(mycobot_mode);
}

void maincontrol_IO()
{
  int pin_data = digitalRead(control_pin);

  return;
  if (pin_data == 1)
  {
    maincontrol_displayInfo(51);
    delay(error_display_time);
    
    mycobot_mode = 12;
    maincontrol_play();
  }

}

bool maincontrol_checkDataLen()
{

  if (rec_data_len == 0){
    maincontrol_displayInfo(53);
    delay(error_display_time);
    return 0;
  }
  
  if (rec_data_len < 10){
    maincontrol_displayInfo(52);
    delay(error_display_time);
    return 0;
  }
  return 1;
}

void Transponder(){
  EXIT = false;
  Transponder_info();
  delay(200);
  while (1)
  {
    Transponder_readdata(); 
    if(EXIT) {break;}
  }
}

bool Transponder_checkHeader()
{
  u8 bDat;
  u8 bBuf[2] = { 0, 0 };
  u8 Cnt = 0;

  while (true)
  {
    
    if (!Transponder_readSerial(&bDat, 1))
      return 0;
    bBuf[1] = bBuf[0];
    bBuf[0] = bDat;

    if (bBuf[0] == HEADER && bBuf[1] == HEADER)
      break;
    ++Cnt;
    if (Cnt > 10)
      return 0;
  }
  return 1;
}

int Transponder_readSerial(unsigned char* nDat, int nLen)
{
  int Size = 0;
  int rec_data;
  unsigned long t_begin = millis();
  unsigned long t_use;
  
  while (true)
  {
    M5.update();
    if (M5.BtnA.wasReleased()) {
        Transponder_connect_ATOM();
    }
    if(M5.BtnC.wasReleasefor(2000)) { EXIT = true; break;}

    if(Serial.available()>0)
    {
        rec_data = Serial.read();   // readSerial
        Serial2.write(rec_data);
        if (rec_data != IO_REC_WRONG)
        {
          if (nDat)
            nDat[Size] = rec_data;
          ++Size;
          t_begin = millis();
        }
        if (Size >= nLen)
          break;
        t_use = millis() - t_begin;
        if (t_use > IO_TIME_OUT)
          break;
    }
    // read serial 2
    if (Serial2.available() > 0) {     // If anything comes in Serial 2
         Serial.write(Serial2.read());    // read it and send it out Serial (USB)
    } 
  }
  return Size;
}

void Transponder_rFlushSerial()
{
  while (Serial.read() != -1)
    ;
}

int Transponder_readdata()
{
  Transponder_rFlushSerial();
  if (!Transponder_checkHeader())
    return -1;

  u8 data_len[1];
  u8 r_data_4[4];
  if (Transponder_readSerial(data_len, 1) != 1)
    return -1;
    
  switch (static_cast<int>(data_len[0]))
  {
    case 4:
      Transponder_readSerial(r_data_4, 4);
      switch (int(r_data_4[0]))
      {
        case 0xa0:
        { 
            //0xfe 0xfe 0x04 0xa0 pin_no pin_data 0xfa
          
            byte pin_no = r_data_4[1];
            bool pin_data = r_data_4[2];

            digitalWrite(pin_no,pin_data);

        }
        break;
      }
      break;

    default:
      break;
  }
  
  while(Serial.available()>0) {Serial2.write(Serial.read()); }
  while(Serial2.available()>0) {Serial.write(Serial2.read());} 

}

void Transponder_connect_ATOM(){
  M5.Lcd.clear(BLACK);
  delay(50);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(55, 40);
  M5.Lcd.println("Connect test");
  M5.Lcd.setCursor(5, 80);
  int state = myCobot.isPoweredOn();
  if(state == 1){
    M5.Lcd.println("Atom: ok");
  }else{
    M5.Lcd.println("Atom: no");
  }
  delay(2000);
  Transponder_info();
}

void Transponder_info()
{
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("myCobot");
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("Basic Transponder");
  M5.Lcd.setCursor(10, 90);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println("BtnA - test connect");
  M5.Lcd.setCursor(10, 110);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println("BtnC - EXIT");
}
