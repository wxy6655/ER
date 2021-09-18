#include <TransPonder.h>

// MycobotBasic myCobot;

// #define 
typedef  unsigned char u8;

void Transponder::init() {
  // myCobot.setup();
  // myCobot.powerOn();
  
  pinMode(15, OUTPUT); // 1 
  pinMode(5, OUTPUT); // 2
  pinMode(2, OUTPUT); // 2
  delay(100);
  info();
}
  
void Transponder::ponder(MycobotBasic &myCobot) {
  init();
  readData(myCobot);
}

bool Transponder::checkHeader(MycobotBasic &myCobot)
{
  u8 bDat;
  u8 bBuf[2] = { 0, 0 };
  u8 Cnt = 0;

  while (true)
  {
    if (!readSerial(myCobot, &bDat, 1))
      return 0;
    bBuf[1] = bBuf[0];
    bBuf[0] = bDat;

    if (bBuf[0] == header && bBuf[1] == header)
      break;
    ++Cnt;
    if (Cnt > 10)
      return 0;
  }
  return 1;
}

int Transponder::readSerial(MycobotBasic &myCobot, unsigned char* nDat, int nLen)
{
  int EXIT = false;
  int Size = 0;
  int rec_data;
  unsigned long t_begin = millis();
  unsigned long t_use;

  while (true)
  {
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        connect_ATOM(myCobot);
    }
    if(M5.BtnC.wasReleasefor(1000)) { EXIT = true; break;}
    // Serial.print("dadadada");
    if(Serial.available()>0)
    {
      rec_data = Serial.read();   // readSerial
      Serial2.write(rec_data);
      if (rec_data != IORecWrong)
      {
        if (nDat)
          nDat[Size] = rec_data;
        ++Size;
        t_begin = millis();
      }
      if (Size >= nLen)
        break;
      t_use = millis() - t_begin;
      if (t_use > IOTimeOut_1)
        break;
    }
    // read serial 2
    if (Serial2.available() > 0) {     // If anything comes in Serial 2
         Serial.write(Serial2.read());    // read it and send it out Serial (USB)
    } 
  }
  return Size;
}

void Transponder::rFlushSerial()
{
  while (Serial.read() != -1)
    ;
}

int Transponder::readData(MycobotBasic &myCobot)
{
  rFlushSerial();
  if (!checkHeader(myCobot))
    return -1;

  u8 data_len[1];
  u8 r_data_4[4];
  if (readSerial(myCobot, data_len, 1) != 1)
    return -1;
    
  switch (static_cast<int>(data_len[0]))
  {
    case 4:
      readSerial(myCobot, r_data_4, 4);
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

void Transponder::connect_ATOM(MycobotBasic &myCobot){
  M5.Lcd.clear(BLACK);
  delay(50);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(55, 5);
  M5.Lcd.println("Connect test");
  M5.Lcd.setCursor(5, 40);
  int state = myCobot.isPoweredOn();
  if(state == 1){
    M5.Lcd.println("Atom: ok");
  }else{
    M5.Lcd.println("Atom: no");
  }
  M5.update();
  delay(10);
  delay(2000);
  info();
}

void Transponder::info()
{
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("myCobot");
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Basic Transponder");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println("Press A - Test Atom Connect");
  M5.Lcd.println("Press C - EXIT(1S)");
  M5.Lcd.setCursor(40, 210);
  M5.update();
  delay(10);
}
