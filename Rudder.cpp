/*
 * Rudder.cpp
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *  Known Issues:
 *    1. position is given a voltage value which is not useful for display, should
 *       add a mapping function to return degrees.
 */

#include "Rudder.h"
//#include "Motor.h"
#include <Arduino.h>
//#include <PID_v1.h>

#define GOOD 15
#define CURRENT_SENSING_DOWN 20
#define POSITION_SENSING_DOWN 21
#define CURRENT_WARNING
#define TOLERATE 12

double Kp=2, Ki=5, Kd=1;

Rudder::Rudder(int position_p, int pwm_p, int in1_p, int in2_p, int current_p)
	:position_p(position_p), setpoint(0), output(0), position(0),
	 motor(new Motor(pwm_p,in1_p,in2_p,current_p)),
	 pid(new PID(&position, &output, &setpoint, Kp,Ki,Kd,DIRECT))

{
  //the controller should be set to 0 physically
  //stat is initialized to GOOD

  //update position
	update_position();
	pid->SetSampleTime(3000);
	pid->SetOutputLimits(-255,255);
	pid->SetMode(AUTOMATIC);

}

void Rudder::update_position(){
	position = analogRead(position_p);
}

void Rudder::update_setpoint(double setpoint_received){
	setpoint = setpoint_received;
} //setpoint should be updated before Compute_and_Drive()

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
				motor->change_to_reverse();
				motor->set_pwm(-output);
			}
			else{
				motor->change_to_direct();
				motor->set_pwm(output);
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
	update_position();
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
