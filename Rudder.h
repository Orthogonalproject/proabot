/*
 * Rudder.h
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 *  	1. motor and pid should be private. making them public gives too much
 *  	   control to the main script. but it's difficult to calibrate and
 *  	   experiment things with an extra method. They will stay public for now.
 */

#ifndef RUDDER_H
#define RUDDER_H

#include "Motor.h"
#include <PID_v1.h>

class Rudder {
private:
    int position_p;
    double setpoint;
    double output;
    double position;
public:
	Rudder (int position_pin, int pwm_p, int in1_p, int in2_p, int current_p);

	int choose_direction();

	void update_setpoint(double setpoint);
	void update_position();

	void Compute_and_Drive();

	int get_position();
	int get_output();
	int get_stat();
	void set_direction(int direction);
	//void check_current();

	void print();
	~Rudder();
    Motor * motor;
    PID * pid;


};

#endif /* RUDDER_H */
