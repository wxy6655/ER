/*
  motor Control - 控制电机
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define M1PinDir 4
#define M1PinSpeed 3
#define M2PinDir 7
#define M2PinSpeed 5
#define M3PinDir 8
#define M3PinSpeed 6
#define M4PinDir 12
#define M4PinSpeed 11

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(M1PinDir, OUTPUT);
  pinMode(M1PinSpeed, OUTPUT);
  pinMode(M2PinDir, OUTPUT);
  pinMode(M2PinSpeed, OUTPUT);
  pinMode(M3PinDir, OUTPUT);
  pinMode(M3PinSpeed, OUTPUT);
  pinMode(M4PinDir, OUTPUT);
  pinMode(M4PinSpeed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  setMotor(100, 100, 100, 100);    //前进
  delay(1000);
  setMotor(-100, -100, -100, -100);  //后退
  delay(1000);
  setMotor(0, 0, 0, 0);  //后退
  delay(2000);
}

/*
  驱动电机函数
  参数：m1Speed - 1电机速度 m2Speed - 2电机速度
       m3Speed - 3电机速度 m4Speed - 4电机速度 （取值范围：-255 ~ 255）
*/
void setMotor(int m1Speed, int m2Speed, int m3Speed, int m4Speed) {
  if (m1Speed > 0)
    digitalWrite(M1PinDir, LOW);
  else if (m1Speed < 0)
    digitalWrite(M1PinDir, HIGH);
  analogWrite(M1PinSpeed, abs(m1Speed));

  if (m2Speed > 0)
    digitalWrite(M2PinDir, HIGH);
  else if (m2Speed < 0)
    digitalWrite(M2PinDir, LOW);
  analogWrite(M2PinSpeed, abs(m2Speed));

  if (m3Speed > 0)
    digitalWrite(M3PinDir, LOW);
  else if (m3Speed < 0)
    digitalWrite(M3PinDir, HIGH);
  analogWrite(M3PinSpeed, abs(m3Speed));

  if (m4Speed > 0)
    digitalWrite(M4PinDir, HIGH);
  else if (m4Speed < 0)
    digitalWrite(M4PinDir, LOW);
  analogWrite(M4PinSpeed, abs(m4Speed));
}
