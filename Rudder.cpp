/*
 * Rudder.cpp
 *
 *  Created: January 7, 2018
 *  Update:  June 6, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 */

#include "Rudder.h"
#include <Arduino.h>

#define GOOD 15
#define CURRENT_SENSING_DOWN 20
#define POSITION_SENSING_DOWN 21
#define TOLERATE 8
#define POSITION_LOWER_LIMIT  279   //465    //459
#define POSITION_UPPER_LIMIT  575      //680

/*
 * These values are convenient for testing purposes but in real application
 * pid->SetTunings(double,double,double) is called right after the construction
 * to overide them.
 *
 *
 * Notes for r1 tuning: For our purpose, Kp should be in range(10,15)
 * Kp = 8 is accurate for a long distance, but short distance takes a long time
 * final values for R1: 4/28 Kp = 4, Kd = 3, Ki = 1
 */
double Kp=4, Kd = 3, Ki = 1;


/*
 * setpoint is set to 0 in the code. But 0 is actually not in the range of
 * possible values for the position. This value will be updated to a
 * correct value in the first loop.
 * SampleTime, OutputLimits are two settings that can be customized.
 *
 * > pid->SetOutputLimits(-135,135);
 *     OutputLimits should be well chosen for different rudders(motors)
 *     so a more logical way is to adjust this setting every time a rudder
 *     is constructed.
 *
 * pid has two modes: AUTOMATIC, MANUAL
 *     AUTOMATIC mode is used when we use PID to calculate motor output
 *     MANUAL mode is used when we manually define the output
 *       If we don't switch to MANUAL when we define the output, the PID
 *       formula will give inaccurate output because of the wrong assumption
 *       which is that PID's output is being used. More information on this
 *       issue can be found:
 *       http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
 *
 * motor and pid are dynamically allocated here. The memory gets freed in ~Rudder()
 */
Rudder::Rudder(int position_p, int pwm_p, int in1_p, int in2_p, int current_p)
	:position_p(position_p), setpoint(0), output(0), position(0),position_lower_limit(200),position_upper_limit(800),
	 motor(new Motor(pwm_p,in1_p,in2_p,current_p)),
	 pid(new PID(&position, &output, &setpoint, Kp,Ki,Kd,DIRECT))

{
	update_position();
	pid->SetSampleTime(1000);
	pid->SetMode(AUTOMATIC);

}

void Rudder::set_limits(int lower, int upper)
{
	position_lower_limit = lower;
	position_upper_limit = upper;
}

/*
 * analog value will not be stable. We could smooth it out
 * by assigning an average value of a consecutive 10 msec. But
 * since we are only checking it every thousand msec, that might
 * not be necessary. Note: delay cannot be used anywhere in the
 * program because it will cause the NRF module to stop working.
 * which essentially stops the program.
 */
void Rudder::update_position(){
	position = analogRead(position_p);
}


void Rudder::update_setpoint(double setpoint_received){
	setpoint = setpoint_received;
}


void Rudder::Compute_and_Drive(){
	int dir = motor->get_direction();
	if (dir==HIGH_CURRENT){
		pid->SetMode(MANUAL);
		output = 0;
		motor->ask_reboot();
	}
	   //compute_and_drive only operates when NOT(HIGH_CURRENT)
	else{
		update_position();
		if (abs(setpoint-position) < TOLERATE){
			motor->stop_motor();
			pid->SetMode(MANUAL);
			output = 0;
		}

		else{
			pid->SetMode(AUTOMATIC);
			pid->Compute(); //output is computed
			if (output<0){
				if(position<position_lower_limit){
					motor->stop_motor();
					pid->SetMode(MANUAL);
					output = 0;
				}
				else{
					motor->change_to_reverse();
					motor->set_pwm(-output);
				}
			}
			else{ //logical error
				if(position>position_upper_limit){
					motor->stop_motor();
					pid->SetMode(MANUAL);
					output = 0;
				}
				else{
					motor->change_to_direct();
					motor->set_pwm(output);
				}
			}
		}
	}
}

void Rudder::print(){
	Serial.print("\tSetpoint: ");
	Serial.println(setpoint);
	Serial.print("\tPosition: ");
	Serial.println(position);
	Serial.print("\tOutput: ");
	Serial.println(output);
	Serial.print("\t# Direction: ");
	Serial.println(motor->get_direction());
	// current_sensing is printed inside it's method. It should be called right after this so that you can see which current it's showing. Read issues for more detail.
}

int Rudder::get_position(){
	return position;
}

int Rudder::get_output(){
  return output;
}

int Rudder::get_stat(){
	return motor->get_direction();
}



Rudder::~Rudder() {
	delete motor;
	delete pid;
}
