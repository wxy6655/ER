#include "Servocontrol.h"

SMSBL sm;

int Servocontrol::setup(){
    delay(500);
    M5.begin(true, false, true);
    M5.Power.begin();
    dacWrite(25,0);   // disable mic 
    Serial2.begin(BAUD_RATE);
    delay(500);
    sm.pSerial = &Serial2;
}

int Servocontrol::get_servo_distance(int *old_angles,int *new_angles, int* angles_data){
    int servo_distance = 0;
    for(int i = 0; i < 6; i++){
        angles_data[i] = abs(new_angles[i] - old_angles[i]);
        servo_distance = (servo_distance > angles_data[i]) ? servo_distance : angles_data[i];
    }
    return servo_distance;
}

float Servocontrol::get_servo_run_time(int servo_distance){   
    T_time = ((servo_distance + (speed_MAX * (speed_MAX / acc_MAX)))/speed_MAX);           // x_all = V(T2 + T1)   
    return T_time;
}

double * Servocontrol::get_servo_run_acc(int distance_data, int max_distance){
    static double data[2];
    float k = float(distance_data)/float(max_distance);
    data[0] = k * speed_MAX;
    data[1] = data[0]/4;
//    
//    Serial.print("  k: ");
//    Serial.print(k);
//    Serial.print("  distance_data: ");
//    Serial.print(distance_data);
//    Serial.print("  max_distance: ");
//    Serial.print(max_distance);
//    Serial.print("  speed_MAX: ");
//    Serial.print(speed_MAX);
//    Serial.print("  speed: ");
//    Serial.print(data[0]);
//    Serial.print("  acc: ");
//    Serial.println(data[1]);
    return data;
}

int * Servocontrol::get_servo_angles(){
    static int angles[6];
    for(int i = 0; i <6; i++){
        angles[i] = sm.ReadPos(i+1);
        delay(15);
    }
    return angles;
}

void Servocontrol::set_servo_angles(int *angles, int _speed_MAX){
    int *pAngles;
    int max_distance = 0;
    if(_speed_MAX < 500){
      speed_MAX = 500;
    }else if (_speed_MAX > 500 && _speed_MAX < 2500 ){
      speed_MAX = _speed_MAX;
    }else if( _speed_MAX > 2500){
      speed_MAX = 2500;
    }
    acc_max_time = speed_MAX/acc_MAX;
    float time_1 = 0;
    static int angles_data[6];
    pAngles = get_servo_angles();
    max_distance = get_servo_distance( pAngles, angles, angles_data);
    time_1 = get_servo_run_time( max_distance);
    Serial.println("");
    for (int i = 0; i < 6; i++) {
        double *ptr = get_servo_run_acc(angles_data[i], max_distance);
        sm.WritePosEx(i+1, angles[i], ptr[0], ptr[1]);
        delay(15);
    }
}
