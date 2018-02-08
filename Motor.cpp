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



Motor::Motor(int pwm_p,int in1_p,int in2_p,int current_p)
	:in1_p(in1_p),in2_p(in2_p),pwm_p(pwm_p),direction(STOP), current_p(current_p)
{

  //set up
  pinMode(pwm_p,OUTPUT);
  pinMode(in1_p,OUTPUT);
  pinMode(in2_p,OUTPUT);
  pinMode(current_p,INPUT);
}

void Motor::set_pwm(int pwm){
	if (direction!=HIGH_CURRENT)
		analogWrite(pwm_p,pwm);
}

void Motor::change_to_direct(){
	if(direction!=HIGH_CURRENT){
		  digitalWrite(in1_p,HIGH);
		  digitalWrite(in2_p,LOW);
		  direction = DIRECT;
	}

}

void Motor::change_to_reverse(){
	if (direction!=HIGH_CURRENT){
		digitalWrite(in1_p,LOW);
		digitalWrite(in2_p,HIGH);
		direction = REVERSE;
	}
}

int Motor::get_direction(){
  return direction;
}

void Motor::stop_motor(){
  set_pwm(0);
  digitalWrite(in2_p,LOW);
  digitalWrite(in1_p,LOW);
  direction = STOP;
}

void Motor::sense_current(){
	int current = analogRead(current_p);
	Serial.println(current);
	if(direction!= HIGH_CURRENT && current>CURRENT_THRESHOLD){
		stop_motor();
		direction = HIGH_CURRENT;
		Serial.println("WARNING: HIGH_CURRENT[Motor Stopped.]");
	}
	else if(direction == HIGH_CURRENT){
		Serial.println("[R]eboot Motor? ");
		if(Serial.available()){
			if(Serial.read()=='R')
				direction = STOP;
		}
	}

}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}
