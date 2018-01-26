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
	Motor(int pwm_p, int in1_p, int in2_p);
	void set_pwm(int pwm);
	void change_to_direct();
	void change_to_reverse();
	int get_direction();
	void stop_motor();

	virtual ~Motor();

private:
  int _in1_p;
  int _in2_p;
  int _pwm_p;
  int _direction;
};

#endif /* MOTOR_MOTOR_H_ */
