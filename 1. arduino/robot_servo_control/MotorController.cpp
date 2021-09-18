#include <MotorController.h>




MotorController::MotorController(){
	fl.pSerial = &Serial1;
	rl.pSerial = &Serial2;
	Serial1.begin(BAUD_RATE);
	Serial2.begin(BAUD_RATE);

	updateServoOffsetsMem();
}


void MotorController::writeLegAngle(int leg_no, int joint_no, double input_angle, double time_spend)
{
	int servo_no = getServoNoFromLeg(leg_no, joint_no); 	// 3 servos in one leg
	double modified_angle = getOffsetAngle(servo_no, input_angle, FORWARD_CAL);	

	/*
	Serial.print("servo no ");
	Serial.print(servo_no);
	Serial.print(",  modified_angle ");
	Serial.print(modified_angle);
	Serial.println("");
	*/
	
	writeRawServoData(servo_no, angleToData(modified_angle), time_spend);
	
}

void MotorController::writeLegAngles(int leg_no, double joint_1_angle, double joint_2_angle, double joint_3_angle, double time_spend)
{
	writeLegAngle(leg_no, 1 , joint_1_angle,  time_spend); // 3 servos in one leg
	writeLegAngle(leg_no, 2 , joint_2_angle,  time_spend);
	writeLegAngle(leg_no, 3 , joint_3_angle,  time_spend);
}


void MotorController::writeHeadAngle(int joint_no, double angle, double time_spend)
{
	//junk code
	
	if (joint_no < 1) joint_no = 1;
	if (joint_no > 2) joint_no = 2;


	int servo_no = HEAD_SERVO_START - 1 + joint_no;

	if (joint_no == 1)
	{
		// define max and min

		double max_head_1 = 38;
		double min_head_1 = -40;
		if (angle > max_head_1) angle = max_head_1;
		if (angle < min_head_1)	angle = min_head_1;
	}
	else
	{
		// define max and min

		double max_head_2 = 90;
		double min_head_2 = -90;

		if (angle > max_head_2) angle = max_head_2;
		if (angle < min_head_2)	angle = min_head_2;
	}

	double modified_angle  = angle * HEAD_SIGNS[joint_no-1] + SERVOS_OFFSETS[servo_no - 1] + BASE_OFFSET;

	writeRawServoData(servo_no, angleToData(modified_angle), time_spend);

}

void MotorController::writeTailAngle(int joint_no, double angle, double time_spend)
{


	if (joint_no < 1) joint_no = 1;
	if (joint_no > 2) joint_no = 2;

	int servo_no = TAIL_SERVO_START - 1 + joint_no;

	if (joint_no == 1)
	{
		// define max and min

		double max_tail_1 = 50;
		double min_tail_1 = -50;
		if (angle > max_tail_1) angle = max_tail_1;
		if (angle < min_tail_1)	angle = min_tail_1;
	}
	else
	{
		// define max and min
		
		double max_tail_2 = 120;
		double min_tail_2 = -120;
		if (angle > max_tail_2) angle = max_tail_2;
		if (angle < min_tail_2)	angle = min_tail_2;
	}

	double modified_angle  = angle * TAIL_SIGNS[joint_no-1] + SERVOS_OFFSETS[servo_no - 1] + BASE_OFFSET;

	writeRawServoData(servo_no, angleToData(modified_angle), time_spend);

}

int MotorController::getServoNoFromLeg(int leg_no, int joint_no)
{
	return (leg_no -1 ) * 3 + joint_no;
}

void MotorController::writeRawServoData(int servo_no, int data_raw, int time_spend)
{
	bool isFrontLeg = 1;
	if (servo_no > F_LEGS_SERVO_CT) {isFrontLeg = 0;}	
	if ((servo_no > HEAD_SERVO_START-1) && (servo_no < TAIL_SERVO_START) )
		{isFrontLeg = 1;}	

	if(isFrontLeg)
	{
		fl.WritePos(servo_no,data_raw,time_spend);
	}
	else
	{
		rl.WritePos(servo_no,data_raw,time_spend); 
	}
}

int MotorController::angleToData(double raw_angle)
{
	return int(raw_angle*RESOLUTION); 
}

double MotorController::dataToAngle(int raw_data)
{
	return (double(raw_data))/RESOLUTION; 
}


double MotorController::getOffsetAngle(int servo_no, double angle_raw, bool is_forward)
{	

	double offset, _offset_angle;
	double sign;

	//double legs_offsets[] = {-7.20, -4.69, -36.33, 9.38, 10.55, 49.22, 0.00, 66.80, 88.54, 3.22, 32.81, -75.35	};

	offset = SERVOS_OFFSETS[servo_no-1] + BASE_OFFSET;

	sign = LEG_SIGNS[servo_no-1];
	

	if (is_forward){

		angle_raw = getServoLimit(servo_no, angle_raw);

		return (angle_raw * sign + offset);
	}
	else{
		return (angle_raw - offset)/sign;	// wrong
	}
}

void MotorController::getAngleOffsets()
{
	Serial.println("-------------------");
	Serial.println("-Legs--------------");

	double offsets[LEGS_SERVO_COUNT];

	for (int leg = 1; leg<5; leg++)
	{
		for (int joint = 1; joint < 4; joint++)
		{
			short data_get = getRawPose(leg,joint);
			double angle_offset = (data_get - BASE_OFFSET*RESOLUTION)/RESOLUTION;

			if (joint == 3) 
			{
				if(leg == 3)
				{
					angle_offset += RARE_LEG_ANGLE_MEASURE;
				}
				else if(leg == 4)
				{
					angle_offset -= RARE_LEG_ANGLE_MEASURE;
				}
			}

			int servo_no = (leg-1)*3 + joint;
			offsets[servo_no-1] = angle_offset;

			Serial.print("Leg: ");
			Serial.print(leg);
			Serial.print("; Joint: ");
			Serial.print(joint);
			Serial.print("; Offset is:  ");
			Serial.println(angle_offset,2);
		}
		Serial.println("---");
	}

	Serial.print("Offsets:    ");
	for(int i = 0; i < LEGS_SERVO_COUNT; i ++ )
	{
		Serial.print(offsets[i]);
		if (i != LEGS_SERVO_COUNT-1) Serial.print(", ");
	}
	Serial.println();


	Serial.println("-Head--------------");

	Serial.print("Head 1: ");
	Serial.println( (fl.ReadPos(HEAD_SERVO_START) - BASE_OFFSET*RESOLUTION)/RESOLUTION,2);
	Serial.print("Head 2: ");
	Serial.println( (fl.ReadPos(HEAD_SERVO_START+1) - BASE_OFFSET*RESOLUTION)/RESOLUTION,2);

	Serial.println("-Tail--------------");
	Serial.print("Tail 1: ");
	Serial.println( (rl.ReadPos(TAIL_SERVO_START) - BASE_OFFSET*RESOLUTION)/RESOLUTION,2);
	Serial.print("Tail 2: ");
	Serial.println( (rl.ReadPos(TAIL_SERVO_START+1) - BASE_OFFSET*RESOLUTION)/RESOLUTION,2);

	Serial.println("-End--------------");

}

void MotorController::setServoCalibrationEEPROM()
{

	// legs
	for (int leg = 1; leg<5; leg++)
	{
		for (int joint = 1; joint < 4; joint++)
		{
			short data_get = getRawPose(leg,joint);
			double angle_offset = (data_get - BASE_OFFSET*RESOLUTION)/RESOLUTION;

			if (joint == 3) 
			{
				if(leg == 3)
				{
					angle_offset += RARE_LEG_ANGLE_MEASURE;
				}
				else if(leg == 4)
				{
					angle_offset -= RARE_LEG_ANGLE_MEASURE;
				}
				if(leg == 1)
				{
					angle_offset -= FRONT_LEG_ANGLE_MEASURE;
				}
				else if(leg == 2)
				{
					angle_offset += FRONT_LEG_ANGLE_MEASURE;
				}
			}
			int servo_no = (leg-1)*3 + joint;
			sOffsets.servo_offset[servo_no-1] = angle_offset;
		}
	}

	// junk code 

	// head
	sOffsets.servo_offset[HEAD_SERVO_START-1] = 0;//(fl.ReadPos(HEAD_SERVO_START) - BASE_OFFSET*RESOLUTION)/RESOLUTION;
	sOffsets.servo_offset[HEAD_SERVO_START] = 0;//(fl.ReadPos(HEAD_SERVO_START+1) - BASE_OFFSET*RESOLUTION)/RESOLUTION;

	// tail
	sOffsets.servo_offset[TAIL_SERVO_START-1] =0;// (rl.ReadPos(TAIL_SERVO_START) - BASE_OFFSET*RESOLUTION)/RESOLUTION;
	sOffsets.servo_offset[TAIL_SERVO_START] = 0;//(rl.ReadPos(TAIL_SERVO_START+1) - BASE_OFFSET*RESOLUTION)/RESOLUTION;

	// clear all data
	msp.clearAll();
	Serial.println("Clear all data! ");

	// save
	msp.writeServoOffsets(sOffsets);
	Serial.println("Calibration and save complete! ");

	// update
	updateServoOffsetsMem();
	Serial.println("Update servo offsets! ");
	Serial.println("Done ");

}

void MotorController::updateServoOffsetsMem()
{

	bool check_trigger = msp.getWriteParameter();

	if (check_trigger)
	{
		sOffsets = msp.getServoOffsets();
		
		for(int i = 0 ; i < SERVO_TOTAL_NOS; i ++)
		{
			SERVOS_OFFSETS[i] = sOffsets.servo_offset[i];
		}
	}
	else
	{
		Serial.println("Error: No Calibration data");
	}

}

void MotorController::printServoOffsets()
{
	for(int i = 0 ; i < SERVO_TOTAL_NOS; i ++)
	{
		Serial.print("Servo ");
		Serial.print(i+1);
		Serial.print(", Offset is:");
		Serial.println(SERVOS_OFFSETS[i]);
	}
}

void MotorController::checkServoCalibrationEEPROM()
{
	
	Serial.println("Check Calibration");

	bool check_trigger = msp.getWriteParameter();
	if (check_trigger)
	{
		Serial.println("Check is true, calibrated");
	}
	else
	{
		Serial.println("No Calibration");
	}

	sOffsets = msp.getServoOffsets();

	for(int i = 0; i < SERVO_TOTAL_NOS; i ++ )
	{
		Serial.print("Servo no: ");
		Serial.print(i+1);
		Serial.print(", Offset is: ");
		Serial.print(sOffsets.servo_offset[i]);
	}
	Serial.println();

}


double MotorController::getServoLimit(int servo_no, double input_angle)
{
	double use_angle = input_angle;

	double max_limit[] = {20,70,10,		20,70,10,		20,45,25,		20,45,25	};
	double min_limit[] = {-20,-60,-80,	-20,-60,-80,		-20,-70,-220,	-20,-70,-220	};  // -85

	double use_max = max_limit[servo_no-1];
	double use_min = min_limit[servo_no-1];

	if(use_angle > use_max ) 
	{
		
		use_angle = use_max;
	}
	if (use_angle < use_min)
	{
	
		use_angle = use_min;
	}

	return use_angle;

}

short MotorController::getRawPose(int leg_no, int joint_no)
{
	
	int servo_no = getServoNoFromLeg(leg_no, joint_no);
	short data_raw;

	bool isFrontLeg = 1;
	if (servo_no > F_LEGS_SERVO_CT) {isFrontLeg = 0;}		// there are 6 servos in front legs

	if(isFrontLeg){  
		data_raw = fl.ReadPos(servo_no);
	}
	else{
		data_raw = rl.ReadPos(servo_no);
	}

	return data_raw;
}

double MotorController::getLegAngle(int leg_no, int joint_no)
{
	// step 1 - get raw data
	short data_raw = getRawPose(leg_no, joint_no);
	int servo_no = getServoNoFromLeg(leg_no, joint_no);


	// step 2 - data to angle
	double angle_raw = dataToAngle(data_raw);  // non linear input, need to calculate again
	// mean error - 0.4~0.7 degree

	// step 3 - add offset
	return getOffsetAngle(servo_no, angle_raw, BAKCWARD_CAL);

}

void MotorController::initializeAll()
{
	writeLegAngles(1,0,0,FRONT_LEG_ANGLE_MEASURE,DEFAULT_LONG_TIME);
	writeLegAngles(2,0,0,FRONT_LEG_ANGLE_MEASURE,DEFAULT_LONG_TIME);
	writeLegAngles(3,0,0,-RARE_LEG_ANGLE_MEASURE,DEFAULT_LONG_TIME);
	writeLegAngles(4,0,0,-RARE_LEG_ANGLE_MEASURE,DEFAULT_LONG_TIME);

}


