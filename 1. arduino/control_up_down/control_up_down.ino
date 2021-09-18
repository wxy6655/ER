#include <MycobotBasic.h>
#include <ParameterList.h>
#include <SCServo.h>
#include <ArduinoJson.h>
#include <string>

#define    rev_size    256


HardwareSerial m_serial(1); 
DynamicJsonDocument  buff(512);

int tar_x, base_x_0, base_x_1;
int tar_y, base_y_0, base_y_1;

char buf[rev_size];



MycobotBasic myCobot;
int k = 1;
SMSBL sm;

Coords pos_data;
Angles angles_data;

int power_pin = 2;
int open_pin = 5;
int out_state = 1;
double angles_init[4][6] = { {0.61, -22.41, -128.67, 58.88, 94.30, 1.31},
                          {48.86, -13.53, -77.69, -6.15, 96.67, -2.46},
                          {0.61, -13.53, -77.69, -6.15, 96.67, -2.46},
                          {65.39, -35.24, -87.36, 129.99, -58.44, -117.50+20}
                          };

double angles_csd[8][6] = {{65.39, -35.24, -87.36, 129.99, -58.44, -117.50+20},
                        {58.71, -27.33, -102.39, 134.47, -51.41, -115.66+20},
                        {49.65, -19.42, -112.50, 137.54, -42.18, -114.16+20},
                        {36.56, -16.52, -117.07, 138.60, -29.44, -112.50+20},
                        {21.18, -18.10, -115.13, 137.19, -13.97, -111.53+20},
                        {5.09, -23.20, -108.19, 134.82, 2.02, -110.30+20},
                        {-9.93, -29.70, -95.97, 130.34, 17.22, -108.63+20},
                        {-22.14, -39.19, -78.75, 123.66, 29.35, -107.31+20}};                          

double coords_1[3][6] = {{190.00, -95.50, 249.70, 132.04, -80, -39.51},
                      {190.30, 17.80, 229.50, 125.66, -80, -4.02},
                      {190.30, 95.70, 220.50, 170.81, -80, -27.79}};


                      

void setup() {
  myCobot.setup();
  M5.Lcd.setTextSize(3);
  myCobot.powerOn();
  pinMode(power_pin, OUTPUT);
  pinMode(open_pin, OUTPUT);
  //串口的开启，这里还可以传一些别的参数，但是我们只传入下面四个最重要的：波特率，默认SERIAL_8N1为8位数据位、无校验、0位停止位，后面两个分别为 RXD,TXD 引脚
  m_serial.begin(115200, SERIAL_8N1, 22, 21);
  xibeng(1);
  myCobot.setLEDRGB(255, 255, 255);
  Serial.begin(115200);
  delay(100);
  digitalWrite(power_pin, 1);
  init_pos(0);
}

void loop() {
  // put your main code here, to run repeatedly:
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
     if (M5.BtnA.wasReleased()) {
      reless();
      } 
    if (M5.BtnB.wasReleased()) {
//      left_pos(10);
//      right_pos(4);
//      delay(2000);
//      runing_angles();
//      init_pos();
//        runing_csd_1();
        Json_read();
      }
    if (M5.BtnC.wasReleased()) {
//      runing_angles();  
//      left_pos(4);
//        runing_csd_2();
        read_pos();
        init_pos(1);
    }
}

void runing_csd_1()
{
  Angles angles;
  for(int j = 0; j<8; j++){
    for(int k = 0; k < 6; k++){
      angles[k] = angles_csd[7 - j][k];
    }  
    myCobot.writeAngles(angles,20);
    delay(300);
  }
}

void runing_csd_2()
{
  Angles angles;
  for(int j = 0; j<8; j++){
    for(int k = 0; k < 6; k++){
      angles[k] = angles_csd[j][k];
    } 
    myCobot.writeAngles(angles,20);
    delay(300);
  }
}



void init_pos(int c){
  Angles angles;
  Coords coord;
  for(int i = 0; i < 6; i++){
    coord[i] = coords_1[c][i];
  } 
  myCobot.writeCoords(coord, 20);
  delay(2000);
}

void reless()
{
  for(int i = 1; i<7; i++){
    myCobot.setServoData(i, 40, !k);
    delay(30);
  }
  k = !k;
}

void read_Angles()
{
  Angles data2;
  data2 = myCobot.getAngles();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data2[i]);
    Serial.print(", ");
  }
  Serial.println();
  angles_data = data2;
}

void read_pos()
{
  Coords data1;
  data1 = myCobot.getCoords();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data1[i]);
    Serial.print(", ");
  }
  Serial.println();
  pos_data = data1;
}

void down_pos()
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i<8; i++){
    myCobot.writeCoords(data1,20);
    data1[2] -= 10;
    delay(50);
  }
}

void left_pos(int num)
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i < num; i++){
    myCobot.writeCoords(data1,20);
    data1[1] += 10;
    delay(50);
  }
}

void right_pos(int num)
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i<num; i++){
    myCobot.writeCoords(data1,20);
    data1[1] -= 10;
    delay(50);
  }
}

void up_pos()
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for(int i = 0; i<8; i++){
    myCobot.writeCoords(data1,20);
    data1[2] += 12;
    delay(50);
  }
}

void runing_angles()
{
  Angles angles;
  for(int j = 0; j<2; j++){
    for(int k = 0; k < 6; k++){
      angles[k] = angles_init[j][k];
    } 
    myCobot.writeAngles(angles,50);
    delay(2000);
//    down_pos();
//    delay(2000);
    if(j == 0 or j == 2){
//      delay(2000);
      down_pos();
      delay(2000);
      xibeng(0);
      up_pos();
      delay(3000);
    }
    else{
      xibeng(1);
      delay(2000);
    }
    
    Angles pos;
    for(int k = 0; k < 6; k++){
      pos[k] = angles_init[2][k];
      Serial.print(pos[k]);
    } 
    myCobot.writeAngles(pos,50);  
    delay(1000);
  }
}

void xibeng(int out_state){
  Serial.print(out_state);
  int state = out_state;
  switch (out_state)
  {
    Serial.print(state);
  case 0:
    digitalWrite(power_pin, 0);
    digitalWrite(open_pin, 0);
    break;

  case 1:
    digitalWrite(power_pin, 1);
    digitalWrite(open_pin, 1);
    break;
  
  default:
    break;
  }
}


void Json_read(){
  m_serial.flush();
  if(readSerial(&m_serial, (unsigned char*)buf, rev_size)){
    DeserializationError error = deserializeJson(buff, String(buf));
    // Test if parsing succeeds.
    if (error) {
      // Serial.print(F("deserializeJson() failed: "));
      // Serial.println(error.f_str());
      return;
    }else{
      Serial.println(buf);
      // deserializeJson(buff, String(buf));
      JsonObject root = buff.as<JsonObject>();
      const char* sensor = root["color"];
      tar_x = root["color_data"][0];
      tar_y = root["color_data"][1];
      base_x_0 = root["base_level"][0];
      base_y_0 = root["base_level"][1];
      base_x_1 = root["base_level"][2];
      base_y_1 = root["base_level"][3];
//      Serial.println(sensor);
//      Serial.println(tar_x);
//      Serial.println(tar_y);
//      Serial.println(base_x_0);
//      Serial.println(base_y_0);
//      Serial.println(base_x_1);
//      Serial.println(base_y_1); 
    }
    memset(buf, 0, rev_size);
    delay(5);
  }
  else{
    Serial.println("error data");
  } 
}
int readSerial(HardwareSerial *m_serial, unsigned char *nDat, int nLen)  //读串口数据
{
  int Size = 0;
  int rec_data;
  unsigned long t_begin = millis();
  unsigned long t_use;
  rec_data = m_serial->peek();
  if (!nDat && rec_data != '{') {
     return 0;
  }
  while(1){
    rec_data = m_serial->read();
    if (rec_data != IORecWrong) {                                   //检测数据有无错误
      nDat[Size] = rec_data;                                  //取出数据
      Size ++;
      if (rec_data == '}') {
        // Serial.println(rec_data);
        // Serial.println("---");
        break;                                    //记录时间戳
      }
      if (Size >= nLen) { 
//        Serial.println("ggggg");
        break;
      }
    }else{
      // check time out
      t_use = millis() - t_begin;
      if (t_use > 1000) {                                        //时间超过timeout之后默认此次数据传输结束
        Serial.println("---------------------------------------------------------------time-out-------------------------------------------------------------------");
        break;
      }
    }   
    delay(1);
  }
  return Size;
  
}