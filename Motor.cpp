/*
 * Motor.cpp
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 *  1.current sensing is enforced. no way to bypass it. it does give an option to restart the motor, but
 *    if issue remains, it will be stopped at the next cycle. should add a method to bypass it.
 *  2.current reading for different motors are different.
 *
 */

#include <Arduino.h>
#include "Motor.h"

#define DIRECT 0
#define REVERSE 1
#define STOP 5
#define HIGH_CURRENT 7

#define CURRENT_THRESHHOLD 50

Motor::Motor(int pwm_p,int in1_p,int in2_p,int current_p)
	:_in1_p(in1_p),_in2_p(in2_p),_pwm_p(pwm_p),_direction(STOP), _current_p(current_p)
{

  //set up
  pinMode(pwm_p,OUTPUT);
  pinMode(in1_p,OUTPUT);
  pinMode(in2_p,OUTPUT);
}

void Motor::set_pwm(int pwm){
	if (_direction!=HIGH_CURRENT)
		analogWrite(_pwm_p,pwm);
}

void Motor::change_to_direct(){
  digitalWrite(_in1_p,HIGH);
  digitalWrite(_in2_p,LOW);
  _direction = DIRECT;
}

void Motor::change_to_reverse(){
  digitalWrite(_in1_p,LOW);
  digitalWrite(_in2_p,HIGH);
  _direction = REVERSE;
}

int Motor::get_direction(){
  return _direction;
}
void Motor::stop_motor(){
  set_pwm(0);
  digitalWrite(_in2_p,LOW);
  digitalWrite(_in1_p,LOW);
  _direction = STOP;
}

void Motor::sense_current(){
	if(_direction!= HIGH_CURRENT && analogRead(_current_p)>CURRENT_THRESHHOLD){
		stop_motor();
		_direction = HIGH_CURRENT;
		Serial.println("WARNING: HIGH_CURRENT[Motor Stopped.]");
	}
	else if(_direction == HIGH_CURRENT){
		Serial.println("[R]eboot Motor? ");
		if(Serial.available()){
			if(Serial.read()=='R')
				_direction = STOP;
		}
	}

}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}
