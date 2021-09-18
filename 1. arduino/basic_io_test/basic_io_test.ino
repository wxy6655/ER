#include <MycobotBasic.h>
#include <ParameterList.h>

MycobotBasic myCobot;

//
//
//void task1(void * pvParameters) {
//
//    for(;;) {
//        Serial.print("task1 Uptime (ms): ");
//        Serial.println(millis());
//        delay(100);
//    }
//}
//
//void task2(void * pvParameters) {
//
//    for(;;) {
//        Serial.print("task2 Uptime (ms): ");
//        Serial.println(millis());
//        delay(200);
//    }
//}


void setup(){
  myCobot.setup();                                      
  myCobot.powerOn();                                    //启动机械臂
  pinMode(21,OUTPUT);
  pinMode(22,OUTPUT);
  myCobot.setPinMode(39,INPUT);
//  xTaskCreatePinnedToCore(
//                    task1,     /* Function to implement the task */
//                    "task1",   /* Name of the task */
//                    4096,      /* Stack size in words */
//                    NULL,      /* Task input parameter */
//                    1,         /* Priority of the task */
//                    NULL,      /* Task handle. */
//                    0);        /* Core where the task should run */
//
//  // Task 2
//  xTaskCreatePinnedToCore(
//                    task2,     /* Function to implement the task */
//                    "task2",   /* Name of the task */
//                    4096,      /* Stack size in words */
//                    NULL,      /* Task input parameter */
//                    2,         /* Priority of the task */
//                    NULL,      /* Task handle. */
//                    0);        /* Core where the task should run */
}

void loop(){
  Serial.println(myCobot.getDigitalInput(39));
  delay(200);
}
