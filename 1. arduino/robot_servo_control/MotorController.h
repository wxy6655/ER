/******************************************************************************
* File Name          : MotorController.h
* Author             : JunyiSong 
* Updated            : JunyiSong 
* Version            : V0.1.0 (beta) 
* Created Date       : 13 AUG, 2019
* Modified Date      : 13 AUG, 2019
* Description        : 
* License            : 
* Copyright(C) 2019 Elephant Robotics. All right reserved.
*******************************************************************************/


#ifndef motor_library_h
#define motor_library_h

#include <Arduino.h>
#include <SCServo.h>
#include <MarsParameters.h>

#define BAUD_RATE			1000000
#define RESOLUTION			3.41333		// resolution -> 1024/300
#define CENTER_ANGLE_DATA	511			//BASE_OFFSET*RESOLUTION

#define BASE_OFFSET			150			// Assume the base is -150 degree
#define DEFAULT_TIME		5		// ms
#define DEFAULT_LONG_TIME	200

#define F_LEGS_SERVO_CT		6			// front lges servos count -> 6

#define FORWARD_CAL			1			// use to write angles
#define BAKCWARD_CAL		0			// use to get angles

#define RARE_LEG_ANGLE_MEASURE	90		// rare leg joint 3 measure 70 degree
#define FRONT_LEG_ANGLE_MEASURE	0		// front leg joint 3 measure 20 degree

#define LEGS_SERVO_COUNT	12
#define HEAD_SERVO_START	13
#define TAIL_SERVO_START	15





class MotorController
{
public:
	SCSCL fl, rl;	// front legs,  rare legs.
	MarsParameters msp;
	MarsParameters::servoOffsets sOffsets;

	// calibration and servo check
	float SERVOS_OFFSETS[SERVO_TOTAL_NOS];
	int LEG_SIGNS[LEGS_SERVO_COUNT] = {-1,1,1,  1,-1,-1,  1,1,1,  -1,-1,-1};
	int HEAD_SIGNS[2] = {1, -1};
	int TAIL_SIGNS[2] = {-1, -1};

	MotorController();

	void writeLegAngle(int leg_no, int joint_no, double angle_value, double time_spend);
	void writeLegAngles(int leg_no, double joint_2_angle, double joint_3_angle, double time_spend);

	void writeHeadAngle(int joint_no, double angle, double time_spend);
	void writeTailAngle(int joint_no, double angle, double time_spend);

	double getLegAngle(int leg_no, int joint_no);
	short getRawPose(int leg_no, int joint_no);

	void getAngleOffsets();

	void setServoCalibrationEEPROM();
	void checkServoCalibrationEEPROM();

	void printServoOffsets();
	void updateServoOffsetsMem();


	void initializeAll();


private:

	void writeRawServoData(int servo_no, int data_raw, int time_spend);
	double getOffsetAngle(int servo_no, double angle_raw, bool is_forward);

	int angleToData(double angle_raw);
	double dataToAngle(int data);

	int getServoNoFromLeg(int leg_no, int joint_no);
	double getServoLimit(int servo_no, double input_angle);

};

#endif 