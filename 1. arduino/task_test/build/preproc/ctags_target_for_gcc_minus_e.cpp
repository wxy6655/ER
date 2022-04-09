# 1 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino"
# 2 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino" 2
# 3 "c:\\Users\\MSI_NB\\Desktop\\task_test\\task_demo\\task_demo.ino" 2

HardwareSerial hw_serial(1);
// Callback methods prototypes
void mySerial_read();
void mySerial_write();
void Par();

//Tasks (delay_ms,times,func)
Task t4();
Task t1(1, (-1), &mySerial_read);
Task t2(1, (-1), &mySerial_write);
Task t3(1UL, 1, &Par);

Scheduler runner;


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
  hw_serial.begin(115200, 0x800001c, 33, 32);
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
            pinMode(inIO[i], 0x05);
            delay(20);
        }
        pinMode(outIO[i], 0x02);
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
