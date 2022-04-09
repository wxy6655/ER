#include <Arduino.h>
#line 1 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
#include <TaskScheduler.h>
#include <HardwareSerial.h>

HardwareSerial hw_serial(1);
// Callback methods prototypes
void mySerial_read();
void mySerial_write();
void Par();

//Tasks (delay_ms,times,func)
Task t4();
Task t1(1, TASK_FOREVER, &mySerial_read);
Task t2(1, TASK_FOREVER, &mySerial_write);
Task t3(TASK_MILLISECOND, TASK_ONCE, &Par);

Scheduler runner;


#line 40 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
void setup();
#line 63 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
void IoInit();
#line 87 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
void loop();
#line 19 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
void mySerial_read() {
    while (hw_serial.available())
    {
        Serial.write(hw_serial.read());
    }
}

void mySerial_write() {
    while (Serial.available())
    {
        hw_serial.write(Serial.read());
    }
  
}

void Par() {
    Serial.print("t3: ");
    Serial.println(millis());
  
}

void setup () {
  Serial.begin(115200);
  hw_serial.begin(115200, SERIAL_8N1, 33, 32);
  IoInit();
  Serial.println("Serial open");
  Serial.println("hw_serial open");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1");
  
  runner.addTask(t2);
  Serial.println("added t2");
  
  t1.enable();
  Serial.println("Enabled t1");

  t2.enable();
  Serial.println("Enabled t2");
}

void IoInit()
{
    int inIO[7] = {34, 35, 39, 38, 37, 36, 21};
    int outIO[11] ={25, 26, 22, 4, 12, 19, 23, 18, 14, 27};

     for (int i = 0; i < 11; i++) {
        if (i < 7) {
            pinMode(inIO[i], INPUT_PULLUP);
            delay(20);
        }
        pinMode(outIO[i], OUTPUT);
        delay(20); 
    }
    
    digitalWrite(25, 0);
    delay(200);
    digitalWrite(22, 1);
    delay(200);
    digitalWrite(4, 1);//上电
    delay(200);
    digitalWrite(26, 1);
    delay(200);
}

void loop () {
  runner.execute();
}

