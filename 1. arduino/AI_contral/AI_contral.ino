#include <MycobotBasic.h>
#include <ParameterList.h>
#include <ArduinoJson.h>
#include <string>


#define    rev_size    256

MycobotBasic myCobot;

HardwareSerial m_serial(1); 

//定义复制大小为512位的串口信息存放地址
DynamicJsonDocument  buff(512);

bool Servo_state = 1;


//定义图像识别返回变量
float tar_x, base_x_0, base_x_1;
float tar_y, base_y_0, base_y_1;
// 基准点0坐标，对应图像识别点0
int robo_x_0 = 100;
int robo_y_0 =  57;
// 基准点1坐标，对应图像识别点1
int robo_x_1 = 220;
int robo_y_1 = -57;
// 末端偏移量，预估y轴向负方向偏移量20mm
int offset_x = 0;
int offset_y = -20;


char buf[rev_size];

int power_pin = 2;
int open_pin = 5;
int out_state = 1;
                    

double coords_1[3][6] = {{220,-57, 120, 90, 0.1, 0.1},
                         {160,  0, 120, 90, 0.1, -90},
                         {100, 57, 120, 90, 0.1, -180}};

Coords pos_init_1 = {135.10, -45.10, 90.10, -179.10, 0.10, -90.10};


Angles angles_0 = {0, 0, 0, 0, 0, 0};
Angles angles_1 = {-90.01, -45, -90.30, 45.09, 90.70, 90.63};
Angles angles_2 = {-0, -45, -90.30, 45.09, 90.70, 90.63};    
Angles angles_3 = {-0, -45, -90.30, 45.09, 90.70, 90.63};                 

void setup() {
  myCobot.setup();
  M5.Lcd.setTextSize(3);
  myCobot.powerOn();
  
  pinMode(power_pin, OUTPUT);
  pinMode(open_pin, OUTPUT);
  set_pump(1);

  m_serial.begin(115200, SERIAL_8N1, 22, 21);
  
  myCobot.setLEDRGB(0, 0, 255);
  Serial.begin(115200);
  delay(100);
  Angles angles;
  myCobot.writeAngles(angles_0,30);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    M5.update(); // need to call update()  
    M5.Lcd.setCursor(0,0);
    int j = 0;
     if (M5.BtnA.wasReleased()) {
      reless();
      } 
    if (M5.BtnB.wasReleased()) {
      myCobot.writeAngles(angles_1,70);
      delay(2000);     
//        read_Angles();
//        myCobot.writeCoords(pos_init_1,30);
    }
    if (M5.BtnC.wasReleased()) {
      myCobot.writeAngles(angles_2,50);
      delay(2000);
      runing_angles();
}
}

void reless()
{
  for(int i = 1; i<7; i++){
    myCobot.setServoData(i, 40, !Servo_state);
    delay(30);
  }
  Servo_state = !Servo_state;
}

Angles read_Angles()
{
  Angles data2;
  data2 = myCobot.getAngles();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data2[i]);
    Serial.print(", ");
  }
  Serial.println();
  return data2;
}

Coords read_pos()
{
  Coords data1;
  data1 = myCobot.getCoords();
  delay(50);
  for(int i = 0; i<7; i++){
    Serial.print(data1[i]);
    Serial.print(", ");
  }
  Serial.println();
  return data1;
}

void down_pos()
{
  read_pos();
  Coords data1;
  data1 = read_pos();
  for(int i = 0; i<10; i++){
    myCobot.writeCoords(data1,20);
    data1[2] -= 10;
    delay(50);
  }
}

void left_pos(int num)
{
  
  Coords data1;
  data1 = read_pos();
  delay(50);
  for(int i = 0; i < num; i++){
    myCobot.writeCoords(data1,15);
    data1[1] += 10;
    delay(50);
  }
}

void right_pos(int num)
{
  Coords data1;
  data1 = read_pos();
  delay(50);
  for(int i = 0; i<num; i++){
    myCobot.writeCoords(data1,15);
    data1[1] -= 10;
    delay(50);
  }
}

void up_pos()
{
  Coords data1;
  data1 = read_pos();
  delay(50);
  for(int i = 0; i<10; i++){
    myCobot.writeCoords(data1,20);
    data1[2] += 12;
    delay(50);
  }
}

void runing_angles()
{
  Angles angles;
  int x, y;
  myCobot.writeAngles(angles_2,50);
  delay(2000);
  if(get_pos(&x, &y) < 0){
    Serial.println(" cannot find ");
    return;
  }else{
    Serial.print("find x:");
    Serial.print(x);
    Serial.print("  find y:  ");
    Serial.println(y);
    Coords tar_pos = read_pos();
    delay(50);
    tar_pos[0] = x;
    tar_pos[1] = y;
    myCobot.writeCoords(tar_pos,50);
    delay(2000);
  }
  set_ogject_state(1);
  myCobot.writeAngles(angles_2,50);
  delay(3000);
  myCobot.writeAngles(angles_3,50);  
  delay(3000); 
  set_ogject_state(0);
   
}

void set_ogject_state(int state){
  down_pos();
  delay(2000);
  set_pump(state);
  up_pos();
  delay(3000);
  
}

void set_pump(int out_state){
    digitalWrite(power_pin, out_state);
    digitalWrite(open_pin, out_state);
}

void dispaly(int mode){
  M5.Lcd.clear(BLACK);
  delay(50);
  switch (mode)
    {
      case 0:
      {
        int buttom_y = 60;
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(20,40);
        M5.Lcd.print("MyCobot - Menu");   
        
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(40,buttom_y);
        M5.Lcd.print("Play");

        M5.Lcd.setCursor(130,buttom_y);
        M5.Lcd.print("Relase");

        M5.Lcd.setCursor(232,buttom_y);
        M5.Lcd.print("Record");
        break;
      }
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
      JsonObject root = buff.as<JsonObject>();
      const char* sensor = root["class"];
      tar_x = root["target_date"][0];
      tar_y = root["target_date"][1];
      base_x_0 = root["base_date"][0];
      base_y_0 = root["base_date"][1];
      base_x_1 = root["base_date"][2];
      base_y_1 = root["base_date"][3];
    }
    memset(buf, 0, rev_size);
    delay(5);
  }
  else{
    Serial.println("connot connect m5stivckv");
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

int get_pos(int* robo_x, int* robo_y){
  int number = 0;
  while(1){
    Json_read();
    if(tar_x != 0 && base_x_0 != 0 && base_x_1 != 0 && tar_y != 0 && base_y_0 != 0 && base_y_1){ 
      float tar_kx = ((tar_x - base_x_0)/(base_x_1 - base_x_0));
      float tar_ky = ((tar_y - base_y_0)/(base_y_1 - base_y_0));
      Serial.print(tar_kx);
      Serial.print("      ");
      Serial.println(tar_ky);
      if(0 < tar_kx && tar_kx < 100 && 0 < tar_ky && tar_ky < 100)
      {
        *robo_x = tar_kx*(robo_x_1 - robo_x_0) + robo_x_0;
        *robo_y = tar_ky*(robo_y_1 - robo_y_0) + robo_y_0;
        Serial.print("GET POS AND X/Y");
        Serial.print(*robo_x);
        Serial.print("      ");
        Serial.println(*robo_y);
        return 0;
      }
     } 
    else{
      number++;    
    }
  if(number > 10){Serial.print("CAN'T GET POS AND X/Y");break;} 
  }
  return -1;
}
