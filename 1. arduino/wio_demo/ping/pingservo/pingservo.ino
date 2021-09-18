
#include <SCServo.h>

SMSBL sm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(1000000);
  sm.pSerial = &Serial1;
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 1; i<8; i++){
    if(sm.Ping(i) != -1){
      Serial.println(sm.Ping(i));
      sm.WritePosEx(i, 1000, 2000, 100);
      delay(1000);
      sm.WritePosEx(i, 3999, 2000, 100);
      delay(1000);
    }
    delay(30);
  }
}
