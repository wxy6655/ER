#include "Free_Fonts.h"
#include <SCServo.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define TFT_GREY 0x5AEB

#define HEADER      0XFE
#define IORecWrong  -1
#define IO_TimeOut  25
typedef  unsigned char u8;
typedef  unsigned long u32;
SMSBL sm;
bool EXIT = false;
int Torque = 1;

u32 pins_d[28] = {BCM0,BCM1,BCM2,BCM3,BCM4,BCM5,BCM6,BCM7,BCM8,BCM9,
                BCM10,BCM11,BCM12,BCM13,BCM14,BCM15,BCM16,BCM17,BCM18,BCM19,
                BCM20,BCM21,BCM22,BCM23,BCM24,BCM25,BCM26,BCM27};

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library



int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

void setup() {
  // put your setup code here, to run once:
  initing();

}

void runing(HardwareSerial &Serial1) {
  initing();
  bool EXIT = false;
  while (!EXIT)
  {
    readData(Serial1);
  }
}




void initing() {
  Serial.begin(115200);
  Serial1.begin(1000000);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(15, OUTPUT); // 1 
  pinMode(5, OUTPUT); // 2
  pinMode(35, INPUT); // 2
  pinMode(36, INPUT); // 2
  delay(100);
  digitalWrite(15, 1); // 1 
  digitalWrite(5, 1); // 2
  info();
}



bool checkHeader(HardwareSerial &Serial1)
{
  u8 bDat;
  u8 bBuf[2] = { 0, 0 };
  u8 Cnt = 0;
  while (true)
  {
    if (!readSerial(Serial1, &bDat, 1))
      return 0;
    bBuf[1] = bBuf[0];
    bBuf[0] = bDat;
    if (bBuf[0] == HEADER && bBuf[1] == HEADER){
      break;
    }
    ++Cnt;
    if (Cnt > 64)
      return 0;
  }
  return 1;
}
int readSerial(HardwareSerial &Serial1, unsigned char* nDat, int nLen)
{

  int Size = 0;
  int rec_data;
  unsigned long t_begin = millis();
  unsigned long t_use;

  while (true)
  {
    if(buttonState1 < 1) {Serial.print("123");}
    if(buttonState3 < 1) { EXIT = true; break;}
    if(Serial.available()>0)
    {
      rec_data = Serial.read();   // readSerial
      Serial1.write(rec_data);

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

      if (t_use > IO_TimeOut)
        break;
    }
    // read serial 2
    if (Serial1.available() > 0) {     // If anything comes in Serial 2
         Serial.write(Serial1.read());    // read it and send it out Serial (USB)
    } 
  }
  return Size;
}

void rFlushSerial()
{
  while (Serial.read() != -1)
    ;
}

int readData(HardwareSerial &Serial1)
{
  rFlushSerial();
  if (!checkHeader(Serial1))
    return -1; 
  u8 data_len[1];
  u8 r_data_4[4];
  u8 r_data_3[3];
  u32 *pin_number = pins_d;
  if (readSerial(Serial1, data_len, 1) != 1)
    return -1;
  
  switch (static_cast<int>(data_len[0]))
  {
    case 4:
      readSerial(Serial1, r_data_4, 4);
      switch (int(r_data_4[0]))
      {
        case 0xa0:
        {
          // FE FE 04 A0 0F 00 FA   示例
          int pin_no = r_data_4[1];
          bool pin_data = r_data_4[2];
          u32 _pin_number =*(pin_number + pin_no);
//          Serial.println(_pin_number);
          pinMode(_pin_number, OUTPUT);
          delay(5);
          digitalWrite(_pin_number, pin_data);
          Serial.write(0xfe);
          Serial.write(0xfe);
          Serial.write(0x04);
          Serial.write(0xa0);
          Serial.write(pin_no);
          Serial.write(pin_data);
          Serial.write(0xfa);
        }
        break;
      }
      break;
    case 3:
      readSerial(Serial1, r_data_3, 3);
      switch (int(r_data_3[0]))
      {
        case 0xa1:
        {
          int pin_no = r_data_3[1];
          u32 _pin_number =*(pin_number + pin_no);
          pinMode(_pin_number, INPUT);
          delay(5); 
          bool pin_state = digitalRead(_pin_number);
          delay(5);
          Serial.write(0xfe);
          Serial.write(0xfe);
          Serial.write(0x04);
          Serial.write(0xa1);
          Serial.write(pin_no);
          Serial.write(pin_state);
          Serial.write(0xfa);
        }
        break;
      }
      break;

    default:
      break;
  }
  
  while(Serial.available()>0) {Serial1.write(Serial.read()); }
  while(Serial1.available()>0) {Serial.write(Serial1.read());} 

}

void info()
{
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK); 
  tft.setFreeFont(FS12);
  tft.drawString("MyCobot",0,10);
  tft.setFreeFont(FM9);
  tft.drawString("WIO Transponder 1.0", 0, 60);
}



void loop() {
  buttonState1 = digitalRead(BUTTON_1);
  buttonState2 = digitalRead(BUTTON_2);
  buttonState3 = digitalRead(BUTTON_3);
  runing(Serial1);
}
