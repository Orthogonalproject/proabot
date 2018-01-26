/*
 * Motor.cpp
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *
 *  Known Issues:
 */

#include <Arduino.h>
#include "Motor.h"

#define DIRECT 0
#define REVERSE 1
#define STOP 5

Motor::Motor(int pwm_p,int in1_p,int in2_p){
  //assign pin number and direction
  this->_in1_p = in1_p;
  this->_in2_p = in2_p;
  this->_pwm_p = pwm_p;
  this->_direction = STOP;

  //set up
  pinMode(pwm_p,OUTPUT);
}

void Motor::set_pwm(int pwm){
  analogWrite(this->_pwm_p,pwm);
}

void Motor::change_to_direct(){
  digitalWrite(this->_in1_p,HIGH);
  digitalWrite(this->_in2_p,LOW);
  this->_direction = DIRECT;
}

void Motor::change_to_reverse(){
  digitalWrite(this->_in1_p,LOW);
  digitalWrite(this->_in2_p,HIGH);
  this->_direction = REVERSE;
}

int Motor::get_direction(){
  return this->_direction;
}
void Motor::stop_motor(){
  this->set_pwm(0);
  this->_direction = STOP;
}


Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

