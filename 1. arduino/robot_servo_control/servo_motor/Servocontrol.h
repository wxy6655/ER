#ifndef Servocontrol_h
#define Servocontrol_h

#include <SCServo.h>
#include <M5Stack.h>
#include <Arduino.h>


//#define getArrayLen(array,len) {len = sizeof(array)/sizeof(array[0]);}
#define BAUD_RATE       1000000


class Servocontrol
{
    
private:
    int speed_MAX = 1000;
    int acc_MAX = 200;
    int acc_max_time = 2;
    float T_time = 0;
    float get_servo_run_time(int servo_distance);
    int   get_servo_distance(int *old_angles,int *new_angles, int* angles_data);
    double *  get_servo_run_acc(int distance_data, int max_distance);
public:
    void set_servo_angles(int *angles, int acc_MAX);
    int* get_servo_angles();
    int  setup();
};

#endif
