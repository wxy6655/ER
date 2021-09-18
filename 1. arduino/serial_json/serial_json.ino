#include <ArduinoJson.h>
#include <MycobotBasic.h>
#include <string>

#define    rev_size    256

MycobotBasic myCobot;

HardwareSerial m_serial(1); 
DynamicJsonDocument  buff(512);

int tar_x, base_x_0, base_x_1;
int tar_y, base_y_0, base_y_1;

char buf[rev_size];

void setup() {
  M5.begin(true, false, true, false);                          //启动M5
  delay(100);                                           //延迟100ms
 
  //串口的开启，这里还可以传一些别的参数，但是我们只传入下面四个最重要的：波特率，默认SERIAL_8N1为8位数据位、无校验、0位停止位，后面两个分别为 RXD,TXD 引脚
  m_serial.begin(115200, SERIAL_8N1, 22, 21);
  M5.Lcd.setTextSize(2);                                //设置lcd的显示文字大小
  myCobot.setup();                                      
  myCobot.powerOn();                                    //启动机械臂
  Angles angles = { 0.0, -0, -0, 0, 0, 0.0 }; 
  myCobot.writeAngles(angles, 80);                      //将机械臂移动至定点
}
 


void loop() { 
  Json_read();
  delay(200);
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
