/*
 * Rudder.h
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 */

#ifndef RUDDER_RUDDER_H_
#define RUDDER_RUDDER_H_

#include "Motor.h"

class Rudder {
public:
	Rudder (int position_p, Motor& m);
	void drive(double power);
	int choose_direction();
	void set_command(double cmd);
	void update_position();
	int get_position();
	int get_power();
	int get_command();
	int get_direction();
	void set_direction(int direction);
	//void check_current();
    double command;
    double power;
    double position;
	virtual ~Rudder();

private:
    int position_p;
    int stat;
    Motor& motor;
};

#endif /* RUDDER_RUDDER_H_ */
