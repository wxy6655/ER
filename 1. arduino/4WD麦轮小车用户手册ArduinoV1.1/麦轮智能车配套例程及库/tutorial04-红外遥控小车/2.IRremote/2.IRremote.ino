/*
  Infrared remote control - 红外遥控小车运动
  LED Pin conflict with serial port, Cannot be used simultaneously
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define SERIALDEBUG

// sound pin number
#define SOUND_PIN    9

// motor pins
#define M1PinDir 4
#define M1PinSpeed 3
#define M2PinDir 7
#define M2PinSpeed 5
#define M3PinDir 8
#define M3PinSpeed 6
#define M4PinDir 12
#define M4PinSpeed 11

#define RECV_PIN 2    //Sets the receive pin for the IR.

#include "IRremote.h"
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long previousMillis = 0;
uint8_t moveMode = 6;

const uint8_t defaultSpeed = 120;
#define maxSpeed  255
#define minSpeed  80
int m1Speed = defaultSpeed;
int m2Speed = defaultSpeed;
int m3Speed = defaultSpeed;
int m4Speed = defaultSpeed;

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  pinMode(M1PinDir, OUTPUT);
  pinMode(M1PinSpeed, OUTPUT);
  pinMode(M2PinDir, OUTPUT);
  pinMode(M2PinSpeed, OUTPUT);
  pinMode(M3PinDir, OUTPUT);
  pinMode(M3PinSpeed, OUTPUT);
  pinMode(M4PinDir, OUTPUT);
  pinMode(M4PinSpeed, OUTPUT);

  // delay(1000);
  
  
  // digitalWrite(M1PinDir, HIGH);
  // analogWrite(M1PinSpeed, abs(m1Speed));

  // while (1)
  // {
  // }



  irrecv.enableIRIn(); // Start the receiver
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif

  MSound(1, 100, 2500);
  setMotor(m1Speed, m2Speed, m3Speed, m4Speed);
  delay(1000);
  setMotor(0, 0, 0, 0);
}







void loop() {
  if (irrecv.decode(&results)) {
    dosomething(&results);
    previousMillis = millis();
    irrecv.resume(); // Receive the next value
  } else {
    if (millis() - previousMillis > 110) {
      previousMillis = millis();
      moveMode = 6;
    }
  }
  // 电机运动模式
  if (moveMode == 0)
    setMotor(m1Speed, m2Speed, m3Speed, m4Speed);      // 前进
  else if (moveMode == 1)
    setMotor(m1Speed, -m2Speed, -m3Speed, m4Speed);    // 右平移
  else if (moveMode == 2)
    setMotor(-m1Speed, m2Speed, m3Speed, -m4Speed);    // 左平移
  else if (moveMode == 3)
    setMotor(-m1Speed, -m2Speed, -m3Speed, -m4Speed);  // 后退
  else if (moveMode == 4)
    setMotor(-m1Speed, m2Speed, -m3Speed, m4Speed);    // 左旋转
  else if (moveMode == 5)
    setMotor(m1Speed, -m2Speed, m3Speed, -m4Speed);    // 右旋转
  else if (moveMode == 6)
    setMotor(0, 0, 0, 0);                              // 停止
}

/*
FF02FD
FF9867
FFE01F
FF906F
*/

void dosomething(decode_results *results) {
#ifdef SERIALDEBUG
  Serial.print(results->value, HEX);
  Serial.println("");
#endif
  switch (results->value) {
    case 0xFF02FD:   // 红外 ∧ 按键   -- 前进
      moveMode = 0;
      break;
    case 0xFF9867:   // 红外 ∨ 按键   -- 后退
      moveMode = 3;
      break;
    case 0xFFE01F:   // 红外 ＜ 按键   -- 左平移
      moveMode = 2;
      break;
    case 0xFF906F:   // 红外 ＞ 按键   -- 右平移
      moveMode = 1;
      break;
  }
#ifdef SERIALDEBUG
  Serial.print(moveMode);
  Serial.println("");
#endif
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

// BUGBUG:: Move to some library...
//==============================================================================
//    SoundNoTimer - Quick and dirty tone function to try to output a frequency
//            to a speaker for some simple sounds.
//    快速而粗糙的音调功能试图向扬声器输出一些频率以获得一些简单的声音。
//==============================================================================
#ifdef SOUND_PIN
void SoundNoTimer(unsigned long duration,  unsigned int frequency) {
#ifdef __AVR__
  volatile uint8_t *pin_port;
  volatile uint8_t pin_mask;
#else
  volatile uint32_t *pin_port;
  volatile uint16_t pin_mask;
#endif
  long toggle_count = 0;
  long lusDelayPerHalfCycle;

  // Set the pinMode as OUTPUT
  pinMode(SOUND_PIN, OUTPUT);

  pin_port = portOutputRegister(digitalPinToPort(SOUND_PIN));
  pin_mask = digitalPinToBitMask(SOUND_PIN);

  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L / (frequency * 2);

  // if we are using an 8 bit timer, scan through prescalars to find the best fit
  while (toggle_count--) {
    // toggle the pin
    *pin_port ^= pin_mask;

    // delay a half cycle
    delayMicroseconds(lusDelayPerHalfCycle);
  }
  *pin_port &= ~(pin_mask);  // keep pin low after stop
}

void MSound(byte cNotes, ...) {
  va_list ap;
  unsigned int uDur;
  unsigned int uFreq;
  va_start(ap, cNotes);

  while (cNotes > 0) {
    uDur = va_arg(ap, unsigned int);
    uFreq = va_arg(ap, unsigned int);
    SoundNoTimer(uDur, uFreq);
    cNotes--;
  }
  va_end(ap);
}
#else
void MSound(byte cNotes, ...)
{
};
#endif
