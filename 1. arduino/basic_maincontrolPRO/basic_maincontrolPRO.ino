#include <M5Stack.h> 
#include <SCServo.h>

SMSBL sm;
int baundrate = 1000000;

typedef struct{
  int joint_angle[6];
} joint_angles_enc;

int data_len_max = 1000;
joint_angles_enc jae[1000];

byte control_pin = 26;   // PIN 26 high -> loop play data from Flash

int rec_data_len = 0;
byte mycobot_mode = 0; 

int error_display_time = 3000;

void setup() {
   M5.begin(true, false, true);
  M5.Power.begin();
  
  M5.Lcd.setTextSize(3);
  Serial.begin(115200);
 
  Serial2.begin(baundrate);
  sm.pSerial = &Serial2;
  
  updateMode(mycobot_mode);
}

void loop() {
   M5.update(); 
   byte btn_pressed = 0;
   
   if (M5.BtnA.wasPressed()) {
        btn_pressed = 1;
        updateMode(btn_pressed);
      } 
   if(M5.BtnB.wasPressed()){

    for(int i = 1; i < 7; i ++)
    {
       sm.unLockEprom(i);//打开EPROM保存功能
       delay(3);
      sm.writeByte(i, 40, 0);//ID
      delay(3);
      sm.LockEprom(i);//关闭EPROM保存功能
      delay(3);
    }
      btn_pressed = 2;
      updateMode(btn_pressed);
     }
   if (M5.BtnC.wasPressed()) {
        btn_pressed = 3;
        updateMode(btn_pressed);
      }
}


void updateMode(byte btn_pressed)
{
  if (mycobot_mode == 0)
  {
    switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 1;
        break;
      case 2:
        mycobot_mode = 0;
        break;
      case 3:
        mycobot_mode = 2;
        break;
    }
    displayInfo(mycobot_mode);
  }
  else if(mycobot_mode == 1)
  {
    
    
     switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 11;
        displayInfo(mycobot_mode);
        play(); // play loop from ram
        break;
      case 2:
        mycobot_mode = 11;  // only for flash now
        play(); // play loop from flash
        break;
      case 3:
        mycobot_mode = 0;   // get back
        displayInfo(mycobot_mode);
        break;
    }
    
  }

  else if(mycobot_mode == 2)
  {
    
    switch (btn_pressed)
    {
      case 1:
        mycobot_mode = 21;
        displayInfo(mycobot_mode);
        record();
        break;
      case 2:
        mycobot_mode = 21;  // record into ram as well
        displayInfo(mycobot_mode);
        record();
        break;
      case 3:
        mycobot_mode = 0;
        displayInfo(mycobot_mode);
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
    displayInfo(mycobot_mode);
  }
  
}

void displayInfo(byte mc_mode)
{
  M5.Lcd.clear(BLACK);
  delay(50);

  int buttom_2y = 190;
  int buttom_y = 210;

  switch (mc_mode){
    case 0:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(20,40);
      M5.Lcd.print("MyCobot Pro");   
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Play");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Relase");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Record");
      break;
    }
    
    case 1:
    {

       M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(20,40);
      M5.Lcd.print("Infinite Playing Mode");   
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(30,buttom_2y);
      M5.Lcd.print("Play");
      M5.Lcd.setCursor(30,buttom_y);
      M5.Lcd.print("Ram");

      M5.Lcd.setCursor(136,buttom_2y);
      M5.Lcd.print("Play");
      M5.Lcd.setCursor(136,buttom_y);
      M5.Lcd.print("Flash");

      M5.Lcd.setCursor(232,buttom_y);
      M5.Lcd.print("Back");

      break;
    
    }
    case 2:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(20,40);
      M5.Lcd.print("Recording Mode"); 
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(30,buttom_2y);
      M5.Lcd.print("Record");
      M5.Lcd.setCursor(30,buttom_y);
      M5.Lcd.print("Ram");

      M5.Lcd.setCursor(136,buttom_2y);
      M5.Lcd.print("Record");
      M5.Lcd.setCursor(136,buttom_y);
      M5.Lcd.print("Flash");

      M5.Lcd.setCursor(240,buttom_y);
      M5.Lcd.print("Back");
      break;
    }
   
    case 11:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Play from Ram\nPlaying...");

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Play");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Pause");

      M5.Lcd.setCursor(230,buttom_y);
      M5.Lcd.print("Stop");
      break;
    }
    case 12:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Play from Flash/nPlaying"); 
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Play");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Pause");

      M5.Lcd.setCursor(230,buttom_y);
      M5.Lcd.print("Stop");
      break;
    }
    
    case 21:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Record into Ram\nRecording..."); 

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(5,buttom_y);
      M5.Lcd.print("Stop Recording and Save");

      break;
    }
    case 22:
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Record into Flash\nRecording..."); 

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(5,buttom_y);
      M5.Lcd.print("Stop Recording and Save");

      break;
    }
    
    case 32: // Stop recording
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Puase Now"); 

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(40,buttom_y);
      M5.Lcd.print("Play");

      M5.Lcd.setCursor(130,buttom_y);
      M5.Lcd.print("Pause");

      M5.Lcd.setCursor(230,buttom_y);
      M5.Lcd.print("Stop");

      break;
    }

    case 33: // Stop recording
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Saved Recording"); 
      break;
    }

    case 41: // Play from Flash
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Getting data from Flash"); 
      break;
    }

    case 42: // Record from Flash
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Saving Data into Flash"); 
      break;
    }

    case 51: // loop play from sram
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("IO Pin Active!");
      break; 
    }

    case 52: // loop play from sram
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Data too short, not playing");
      break; 
    }

    case 53: // loop play from sram
    {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0,40);
      M5.Lcd.print("Empty data!");
      break; 
    }
  }
}

void record()  // is stop
{


  // record mode : 1- record to ram;  2- record to flash
  rec_data_len = 0;

  for (int data_index = 0; data_index <data_len_max ; data_index ++)
  {
    M5.update(); 

    for (int i = 0 ; i < 4; i ++)
    {
      jae[data_index].joint_angle[i] = sm.ReadPos(i+1);
      Serial.println(jae[data_index].joint_angle[i]);
      delay(15);
    }
    rec_data_len++;
    if (M5.BtnA.wasPressed()||M5.BtnB.wasPressed()||M5.BtnC.wasPressed()) break;
  }

  displayInfo(33);
  delay(2000);
  
  mycobot_mode = 0;
  displayInfo(mycobot_mode);
}

void play()  // is stop  is pause
{
 
  bool is_stop = 0;
  bool is_pause = 0;

  if (mycobot_mode == 12)
  {
    M5.update(); 
    displayInfo(41);

    
    M5.update(); 
    displayInfo(mycobot_mode);
  }
  
  while(1)
  {
    // play once
    for (int index = 0 ; index < rec_data_len; index++)
    {
      M5.update(); 
      double encoders[6];
      for(int i = 0; i<4; i++){
        encoders[i] = jae[index].joint_angle[i];
         sm.WritePosEx(i+1,encoders[i],2000,10); //speed and acc
//
//        delay(1);
//        // int mv = sm.ReadMove(i+1);
//         int pos = sm.ReadPos(i+1);
//         int ct = sm.ReadCurrent(i+1);
//         if ((ct > 7000))
//         {
//          //Serial.println("Stop");
//          i = 100;
//          index = rec_data_len+1;
//          is_stop = 1;
//          sm.WritePosEx(i+1,pos,3000,200);
//          break;
//         }
//         delay(2);
      }
      
      // check pause button
      if (M5.BtnB.wasPressed())
      {
        displayInfo(32);
        while(1)
        {
          M5.update(); 
          if (M5.BtnA.wasPressed())
          {
            displayInfo(11);
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
      delay(20*3); 
    }

    // stop button will also end loop
    if(is_stop == 1){
      break;
    }
  }

  mycobot_mode = 1;
  displayInfo(mycobot_mode);
}


bool checkDataLen()
{

  if (rec_data_len == 0){
    displayInfo(53);
    delay(error_display_time);
    return 0;
  }
  
  if (rec_data_len < 10){
    displayInfo(52);
    delay(error_display_time);
    return 0;
  }
  return 1;
}
