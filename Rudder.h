/*
 * Rudder.h
 *	 Rudder class gives an implementation of all the methods required to control
 *	a motor-controlled rudder. The rudder can be operated by PID controller.
 *
 *  Created: January 7, 2018
 *  Update: June 6, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *	Include classes:
 *	   "Motor.h"  --Steven Hu
 *	   <PID_v1.h> --br3ttb
 *	      http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
 *
 *  Known Issues:
 *    1. position is given a voltage value which is not idea for display, should
 *       add a mapping function to return degrees. However, analog values for
 *       different potentiometers are different and the values vary from time to
 *       time so it does raise problems in the testing stage.
 *    2. If NRF24 doesn't receive anything, it will slow the whole system down.
 *    3. motor and pid should be private. making them public gives access allows
 *       outside code to fail the whole system. but it's difficult to calibrate and
 *  	 experiment things with those two being private. They will stay public for
 *  	 testing purposes.
 *    4. Running two rudders under the current setting (SampleTime, double, parameters)
 *       works fine. It still remains a question if the Mega can handle more objects
 *       of this class run simultaneously.
 */

#ifndef RUDDER_H
#define RUDDER_H

#include "Motor.h"
#include <PID_v1.h>

class Rudder {
private:
    int position_p;
    double setpoint; //the rudder position commanded by the controller
    double output;   // pwm value for motor
    double position; // current position(angle) of the rudder
    int position_lower_limit;  //position limits of rudder
    int position_upper_limit;
public:
    /*
     * Constructor initializes all the pin numbers. These pins are all
     * used to control to motor. a circuit diagram is shown in the
     * documentation.
     *
     */

	Rudder (int position_pin, int pwm_p, int in1_p, int in2_p, int current_p);

	void set_limits(int lower, int upper);
	int choose_direction();

	/*
	 * update_setpoint(double) is called after setpoint is received by the
	 * boat. setpoint should be updated before Compute_and_Drive()
	 */
	void update_setpoint(double setpoint);
	void update_position();

	/*
	 * Compute_and_Drive() calculates the output and operates the motor
	 * using PID controller.
	 *
	 * If the difference between setpoint and position is smaller than
	 * the TOLERATE range (see #define), motor is stopped.
	 *
	 *  POSITION_UPPER_LIMIT gives the analog value range of the motor
	 * potentiometer. Any position (value) that exceeds the range established
	 * by UPPER and LOWER LIMIT will cause the motor to stop.
	 *
	 * HIGH_CURRENT will be given to dir if during the last current
	 * sensing process, a high current is detected. Serial monitor
	 * will give an option to reboot the motor. Otherwise, the motor
	 * will not move under any circumstances.
	 *
	 */
	void Compute_and_Drive();

	/*
	 * get_xxx() methods returns the stored value of different private members.
	 */
	int get_position();
	int get_output();
	int get_stat();

	/*
	 * set_direction(int) is called by Compute_and_Drive() to choose a dir.
	 */
	void set_direction(int direction);

	/*
	 * print() prints all information about the rudder object.
	 */
	void print();

	/*
	 * motor and pid should be kept private (more of it in the known issues part).
	 *   These two points will be initialized to a motor object and a PID object
	 *   at runtime.
	 * destructor makes sure the dynamically allocated motor and pid are freed.
	 */
	~Rudder();
    Motor * motor;
    PID * pid;


};

#endif /* RUDDER_H */
