#include <M5Stack.h>

const int blinkPin1 = 15; //定义引脚15
const int blinkPin2 = 5; //定义引脚5
const int inputPin1 = 35; //定义引脚35
const int inputPin2 = 36; //定义引脚36
boolean blink1State = false;   //定义 blink1State 为false状态
boolean blink2State = true;   //定义 blink2State 为false状态  

void task1(void * pvParameters) {

    for(;;) {
        Serial.print("35,36 input state : ");
        Serial.print(digitalRead(inputPin1));
        Serial.println(digitalRead(inputPin2));
        delay(1000);
    }
}

void task2(void * pvParameters) {

    for(;;) {
        Serial.print("15, 5 out state : ");
        Serial.print(blink1State);
        Serial.println(blink2State);
        blink1State = !blink1State;   //反转blink1State的逻辑真或假（false or  true）
        digitalWrite(blinkPin1, blink1State); //数字引脚，设置为blink1State的状态
        blink2State = !blink2State;
        digitalWrite(blinkPin2, blink2State);
        delay(2000);
    }
}
//
//void task3(void * pvParameters) {
//
//    for(;;) {
//        Serial.print("task3 Uptime (ms): ");
//        Serial.println(millis());
//        delay(1000);
//    }
//}


void setup() {

    M5.begin();
    M5.Power.begin();

    pinMode(blinkPin1, OUTPUT);   //设置blinkPin1 为输出模式
    pinMode(blinkPin2, OUTPUT);   //设置blinkPin2 为输出模式
    pinMode(inputPin1, INPUT);   //设置inputPin1 为输RU模式
    pinMode(inputPin2, INPUT);   //设置inputPin2 为输RU模式

    digitalWrite(blinkPin1, blink2State); //数字引脚，设置为blink1State的状态
    digitalWrite(blinkPin2, blink2State); //数字引脚，设置为blink1State的状态

    // Task 1
    xTaskCreatePinnedToCore(
                    task1,     /* Function to implement the task */
                    "task1",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */

    // Task 2
    xTaskCreatePinnedToCore(
                    task2,     /* Function to implement the task */
                    "task2",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */

    // Task 3
//    xTaskCreatePinnedToCore(
//                    task3,     /* Function to implement the task */
//                    "task3",   /* Name of the task */
//                    4096,      /* Stack size in words */
//                    NULL,      /* Task input parameter */
//                    3,         /* Priority of the task */
//                    NULL,      /* Task handle. */
//                    0);        /* Core where the task should run */
}

void loop() {

    M5.update();
}
