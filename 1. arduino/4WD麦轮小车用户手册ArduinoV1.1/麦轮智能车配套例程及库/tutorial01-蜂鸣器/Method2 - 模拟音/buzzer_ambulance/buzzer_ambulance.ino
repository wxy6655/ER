/*
  蜂鸣器模拟救护车

  蜂鸣器模拟救护车报警声音

  This example code is in the public domain.
  www.yfrobot.com
*/

// buzzer connected to pin 9
int buzzerPin = 9;

void setup() {
  // initialize the pin as an output.
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  unsigned char i, j; //定义变量
  while (1) {
    for (i = 0; i < 80; i++) //输出一个频率的声音
    {
      digitalWrite(buzzerPin, HIGH); //发声音
      delay(1);//延时1ms
      digitalWrite(buzzerPin, LOW); //不发声音
      delay(1);//延时1ms
    }

    for (i = 0; i < 100; i++) //输出另一个频率的声音
    {
      digitalWrite(buzzerPin, HIGH); //发声音
      delay(2);//延时2ms
      digitalWrite(buzzerPin, LOW); //不发声音
      delay(2);//延时2ms
    }
  }
}
