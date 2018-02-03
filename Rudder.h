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

#include <PID_v1.h>
#include "Motor.h"

class Rudder {
public:
	Rudder (int position_p, Motor& m, PID& pid);
	int drive();
	void set_command(double command);
	void update_position();
	int get_position();
	int get_power();
	int get_command();
	void check_current();

	virtual ~Rudder();

private:
    int position_p;
    double position;
    int stat;
    double command;
    double power;
    PID& pid;
    Motor& motor;
};

#endif /* RUDDER_RUDDER_H_ */
