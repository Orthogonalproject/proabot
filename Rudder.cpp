/*
 * Rudder.cpp
 *
 *  Created: Jan, 2018
 *      Author: Steven Hu
 *  >>Project Orthogonal  -Proabot
 *  Known Issues:
 */

#include "Rudder.h"
#include <Arduino.h>

#define GOOD 15
#define CURRENT_SENSING_DOWN 20
#define POSITION_SENSING_DOWN 21
#define CURRENT_WARNING
#define CURRENT_LIMIT 250
#define TOLERATE 8

double Kp=2, Ki=5, Kd=1;

Rudder::Rudder(int position_p, Motor& m)
	:position_p(position_p),stat(GOOD),motor(m), command(0),power(0),position(0)
{
  //the controller should be set to 0 physically


  //initialize PID
  //PID _pid(&this->_position, &this->_power, &this->_command, Kp,Ki,Kd,DIRECT);
  //this->_pid.SetOutputLimits(0,255); this is the default in the source code
  //this->_pid.SetSampleTime(100); this is the default setting

  //stat is initialized to GOOD
   //the first update_position might detect error
  //_stat = GOOD;

  //update position
  this->update_position();

}

void Rudder::update_position(){
  this->position = analogRead(this->position_p);

}

int Rudder::get_position(){
	this->update_position();
  return this->position;
}

int Rudder::get_power(){
  return this->power;
}

int Rudder::get_command(){
	return this->command;
}

int Rudder::choose_direction(){

  if (this->stat != GOOD){
    return 1;
  }

  this->update_position();

  //within TOLERATE PID is off
  if (abs(this->command - this->position) < TOLERATE){
    //pid.SetMode(MANUAL);
    motor.stop_motor();
  }

  //direct
  else if (this->command - this->position > TOLERATE){
    //check motor direction, if reverse stop for a round
    if (this->motor.get_direction() == REVERSE){
      Serial.println("stop");
    	this->motor.stop_motor();

      //this->pid.SetMode(MANUAL);
      //this->motor.change_to_direct();
    }
    else if (this->motor.get_direction() == STOP){
    	Serial.println("change to direct");
      this->motor.change_to_direct();
      //this->pid.SetControllerDirection(DIRECT);
      //this->pid.SetMode(AUTOMATIC);
      //this->pid.Compute();
      //this->motor.set_pwm(this->power);
    }
  }

  //reverse
  else if(this->position - this->command > TOLERATE){
    //check motor direction, if direct stop for a round
    if (this->motor.get_direction() == DIRECT){
    	Serial.println("stop");
      this->motor.stop_motor();
      //this->pid.SetMode(MANUAL);
      //this->motor.change_to_reverse();
    }
    else if (this->motor.get_direction() == STOP){
      this->motor.change_to_reverse();

      //this->pid.SetControllerDirection(REVERSE);
      //this->pid.SetMode(AUTOMATIC);
      //this->pid.Compute();
      //this->motor.set_pwm(this->power);
    }
    Serial.print("now motor direction is: ");
    Serial.println(motor.get_direction());
  }

  return 0;

}

void Rudder::set_direction(int direction){
	if(direction==DIRECT)
		motor.change_to_direct();
	else if(direction==REVERSE)
		motor.change_to_reverse();
  else if(direction==STOP)
    motor.stop_motor();
}
//void Rudder::check_current(){
//  int current = analogRead(this->_current_p);
//  Serial.println(current);
//  if ( current > CURRENT_LIMIT){
//    this->_motor.stop_motor();
//    //this->_stat = CURRENT_WARNING;
//  }
//}

void Rudder::set_command(double command){
  this->command = command;
}

void Rudder::drive(double power){
	this->motor.set_pwm(power);
}

int Rudder::get_direction(){
	return this->motor.get_direction();
}
Rudder::~Rudder() {
	// TODO Auto-generated destructor stub
}
