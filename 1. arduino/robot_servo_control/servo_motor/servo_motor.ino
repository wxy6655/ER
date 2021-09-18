#include "Servocontrol.h"

Servocontrol Servo;

void setup(){
    Servo.setup();
    delay(200);
    int angles_1[6] = {2000, 2000, 2000, 2000, 2000, 2000};
    int angles_2[6] = {1000, 2400, 3300, 2800, 900, 4000};
    Servo.set_servo_angles( angles_1, 400);
    delay(15000);
    Servo.set_servo_angles( angles_2, 400);
    delay(15000);
    Servo.set_servo_angles( angles_1, 1000);
    delay(12000);
    Servo.set_servo_angles( angles_2, 1000);
    delay(12000);
    Servo.set_servo_angles( angles_1, 2000);
    delay(8000);
    Servo.set_servo_angles( angles_2, 2000);
    delay(8000);
    Servo.set_servo_angles( angles_1, 3000);
    delay(8000);
}

void loop(){
    
    
}
