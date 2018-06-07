/*
 * Motor.h
 * 	This class is an implementation of all the methods required
 * 	to control a motor using an L298N H-bridge motor driver.
 * 	>> Please reference "Boat Circuit: H-bridge motor driver"
 * 	    for the circuit diagram.
 *
 *  Created: January 5, 2018
 *  Updated: June 6, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 *   1. Current sensing values are different for different
 *      motor/H-bridge. A universal current_threshold
 *      is used because the current sensing values we got from
 *      the two motors/H-bridges are similar and a single
 *      threshold works just fine.
 *
 */

#ifndef MOTOR_H
#define MOTOR_H

#define DIRECT 0        // These are the encodings of:
#define REVERSE 1       //  motor direction
#define STOP 5          //  motor status: stop
#define HIGH_CURRENT 7  // motor high current warning

#define CURRENT_THRESHOLD 190    //define the cut off point of current warning
                                 //It is an analog value (0-255). Any value
                                 // bigger than 190 will trigger the safety
                                 // mechanism to kick in.
class Motor {
public:

	/*
	 * constructor:
	 *    pwm_pin number, in1_pin #, in2_pin #, current_sensing # is passed
	 *   in and initialized.
	 *    pwm_p, in1_p, in2_p are set to be OUTPUT, current_p is set to be
	 *   INPUT
	 */
	Motor(int pwm_p, int in1_p, int in2_p, int current_p);

	/*
	 * Note that pwm value is not stored in motor object as knowing the
	 * information doesn't benefit any motor operators. However, rudder
	 * does store an output value which is the pwm value for motor.
	 *
	 * Exception: when motor is in HIGH_CURRENT mode, this method will
	 * not work. ask_reboot() is the only back door to the safety
	 * mechanism.
	 */
	void set_pwm(int pwm);

	/*
	 * change_to_direct and change_to_reverse changes motor directions.
	 * Note that there is no predefined direction of motor. The two
	 * methods only guarantee to give opposite directions. Motors need
	 * to be calibrated and directions has to be defined before using
	 * these two methods.
	 */
	void change_to_direct();
	void change_to_reverse();

	void stop_motor();

	/*
	 * sense_current() checks if the current sensing analog value is
	 * larger than CURRENT_THRESHOLD. if it is, safety mechanism kicks
	 * in and stops the motor and set motor status to HIGH_CURRENT.
	 *
	 * ask_reboot() is the only way to reset the motor after a
	 * HIGH_CURRENT is issued. It monitors serial port until a 'R' is detected.
	 *
	 */
	void sense_current();
	void ask_reboot();

	int get_direction();


private:
  int in1_p;
  int in2_p;
  int pwm_p;
  int direction;
  int current_p;

};

#endif /* MOTOR_MOTOR_H */
