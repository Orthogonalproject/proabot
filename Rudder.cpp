/*
 * Rudder.cpp
 *
 *  Created: Jan, 2018
 *  Update:  April 16, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *  Known Issues:
 *    1. position is given a voltage value which is not useful for display, should
 *       add a mapping function to return degrees.
 *    2. If NRF24 doesn't receive anything, it will cause the whole system to fail.
 *
 */

#include "Rudder.h"
#include <Arduino.h>

#define GOOD 15
#define CURRENT_SENSING_DOWN 20
#define POSITION_SENSING_DOWN 21
#define CURRENT_WARNING
#define TOLERATE 5
#define POSITION_LOWER_LIMIT  550
#define POSITION_UPPER_LIMIT  774

//Three parameters for PID controller
// For our purpose, we Kp should be in range(10,15)
double Kp=15, Ki=0, Kd=0;

/*
 * setpoint is set to 0 in the code. But 0 is actually not in the range of
 * possible values for the position. This value will be updated to a
 * correct value in the first loop.
 * SampleTime, OutputLimits are two settings that can be customized.
 */
Rudder::Rudder(int position_p, int pwm_p, int in1_p, int in2_p, int current_p)
	:position_p(position_p), setpoint(0), output(0), position(0),
	 motor(new Motor(pwm_p,in1_p,in2_p,current_p)),
	 pid(new PID(&position, &output, &setpoint, Kp,Ki,Kd,DIRECT))

{
	update_position();
	pid->SetSampleTime(3000);
	pid->SetOutputLimits(-135,135);
	pid->SetMode(AUTOMATIC);

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

/*
 * Function is called after setpoint is received by the boat.
 * setpoint should be updated before Compute_and_Drive()
 */
void Rudder::update_setpoint(double setpoint_received){
	setpoint = setpoint_received;
}

/*
 * The function that calculates the output and operates the motor.
 * HIGH_CURRENT will be given to dir if during the last current
 * sensing process, a high current is detected. Serial monitor
 * will give an option to reboot the motor. Otherwise, the motor
 * will not move under any circumstances.
 *  If the difference between setpoint and position is smaller than
 * the TOLERATE range (see #define), motor is stopped.
 *  POSITION_UPPER_LIMIT gives the analog value range of the motor
 * potentiometer. Any position (value) that exceeds the range established
 * by UPPER and LOWER LIMIT will cause the motor to stop.
 */
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
				if(position<POSITION_LOWER_LIMIT+TOLERATE){
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
				if(position>POSITION_UPPER_LIMIT-TOLERATE){
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
	//update_position();
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
