#include <MycobotBasic.h>
#include <SCServo.h>
#include <M5Stack.h>


SMSBL sm;
int baundrate = 1000000;

MycobotBasic myCobot;

typedef struct{
  int joint_angle[6];
} joint_angles_enc;

int data_len_max = 1000;
joint_angles_enc jae[1000];

int rec_data_len = 0;
byte mycobot_mode = 0; 

byte record_pin = 21;
byte play_pin = 22;

int error_display_time = 3000;

void setup() {
  myCobot.setup();
  SD.begin();
  M5.Lcd.setTextSize(3);
  Serial.begin(9600);
  myCobot.powerOn();
  pinMode(record_pin, INPUT);
  pinMode(play_pin, INPUT);
  myCobot.setLEDRGB(0,255,0);

  Serial2.begin(baundrate);
  sm.pSerial = &Serial2;
  
}

void loop() {
   M5.update();
   if (digitalRead(record_pin) and digitalRead(play_pin)){
    delay(200);
    if (digitalRead(record_pin) and digitalRead(play_pin)){
      Serial.print(digitalRead(record_pin));
      Serial.print(digitalRead(play_pin));
      Serial.println(" No signal input");
    }
   }
   else if (!digitalRead(record_pin) and digitalRead(play_pin)){
    delay(200); 
    if (!digitalRead(record_pin) and digitalRead(play_pin)){
      Serial.print(digitalRead(record_pin));
      Serial.print(digitalRead(play_pin));
      Serial.println(" Start to Record");
      record();  
     } 
   }
   else if (digitalRead(record_pin) and !digitalRead(play_pin)){
    delay(200);
    if (digitalRead(record_pin) and !digitalRead(play_pin)){
      Serial.print(digitalRead(record_pin));
      Serial.print(digitalRead(play_pin));
      Serial.println(" Start to Play");
      playFromFlash();
    }
   }
   else if(!digitalRead(record_pin) and !digitalRead(play_pin)){
    delay(1000);
    if(!digitalRead(record_pin) and !digitalRead(play_pin)){
      Serial.print(digitalRead(record_pin));
      Serial.print(digitalRead(play_pin));
      Serial.println(" Reless signal input");
      myCobot.setFreeMove();
      delay(200);
      myCobot.setLEDRGB(255,0,0);
     }
   }
}



void record()  // is stop
{
  myCobot.setLEDRGB(255,255,0);
  // record mode : 1- record to ram;  2- record to flash
  rec_data_len = 0;
  for (int data_index = 0; data_index <data_len_max ; data_index ++)
  {
    for (int i = 0 ; i < 6; i ++)
    {
      jae[data_index].joint_angle[i] = sm.ReadPos(i+1);
      //Serial.println(jae[data_index].joint_angle[i]);
      if (digitalRead(record_pin)) break;
      delay(REC_TIME_DELAY - SEND_DATA_GAP);
    }
    rec_data_len++;
    if (digitalRead(record_pin)){
      recordIntoFlash();
      break;
    }
    
  }
  Serial.println("rec data len" + String(rec_data_len));
  delay(1000);
}

void play()  // is stop  is pause
{
  myCobot.setLEDRGB(0,255,0);
  M5.update(); 
  delay(20);
  while(1)
  {    //if (rec_data_len == 0) break;
    for (int index = 0 ; index < rec_data_len; index++)
    {

      double encoders[6];

      bool rec_is_wrong = 0;
      for(int i = 0; i<6; i++){
        //Serial.println(jae[index].joint_angle[i]);
        encoders[i] = jae[index].joint_angle[i];
        if (encoders[i] != -1)
        {
          sm.WritePosEx(i+1,encoders[i],2000,10);
        }
        delay(20);
        if (digitalRead(play_pin)) break;
      }
      if (digitalRead(play_pin)) break;
    }
    if (digitalRead(play_pin)) break;
  }
  Serial.println("Finish Play");
}

void playFromFlash()
{
    M5.update(); 
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
    play();
}


void recordIntoFlash()
{
    // recording data
    M5.update(); 
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
}

bool checkDataLen()
{
  M5.Lcd.clear(BLACK);
  delay(50);
  if (rec_data_len == 0){
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(0,40);
    M5.Lcd.print("Empty data!");
    delay(error_display_time);
    return 0;
  }
  
  if (rec_data_len < 10){
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(0,40);
    M5.Lcd.print("Data too short, not playing");
    delay(error_display_time);
    return 0;
  }
  return 1;
}
