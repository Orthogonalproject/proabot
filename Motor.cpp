/*
 * Motor.cpp
 *
 *  Created: January 5, 2018
 *  Updated: June 6, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
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
	Serial.print("\t# Current: ");
	Serial.println(current);
	if(direction!= HIGH_CURRENT && current>CURRENT_THRESHOLD){
		stop_motor();
		direction = HIGH_CURRENT;
		Serial.println("WARNING: HIGH_CURRENT[Motor Stopped.]");
	}

}

void Motor::ask_reboot()
{
	Serial.println("[R]eboot? ");
	if(Serial.read()=='R')
		direction = STOP;
}

