/*
 * Motor.h
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_



class Motor {
public:
	Motor(int pwm_p, int in1_p, int in2_p, int current_p);
	void set_pwm(int pwm);
	void change_to_direct();
	void change_to_reverse();
	void stop_motor();
	void sense_current();

	int get_direction();

	virtual ~Motor();

private:
  int in1_p;
  int in2_p;
  int pwm_p;
  int direction;
  int current_p;

  //don't need to access pwm value when operating
  //motor, so pwm value is stored in rudder objects
  //as power
};

#endif /* MOTOR_MOTOR_H_ */
