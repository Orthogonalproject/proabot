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

Rudder::Rudder(int position_p, Motor& m, PID &pid)
	:position_p(position_p),position(0),motor(m),pid(pid),power(0),command(0),stat(GOOD)
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
  return this->position;
}

int Rudder::get_power(){
  return this->power;
}

int Rudder::get_command(){
	return this->command;
}

int Rudder::drive(){

  if (this->stat != GOOD){
    return 1;
  }

  this->update_position();

  //within TOLERATE PID is off
  if (abs(this->command - this->position) < TOLERATE){
    pid.SetMode(MANUAL);
    motor.stop_motor();
  }

  //direct
  else if (this->command - this->position > TOLERATE){
    //check motor direction, if reverse stop for a round
    if (this->motor.get_direction() == REVERSE){
      this->motor.stop_motor();
      this->pid.SetMode(MANUAL);
      this->motor.change_to_direct();
    }
    else{
      this->motor.change_to_direct();
      this->pid.SetControllerDirection(DIRECT);
      this->pid.SetMode(AUTOMATIC);
      this->pid.Compute();
      this->motor.set_pwm(this->power);
    }
  }

  //reverse
  else{
    //check motor direction, if direct stop for a round
    if (this->motor.get_direction() == DIRECT){
      this->motor.stop_motor();
      this->pid.SetMode(MANUAL);
      this->motor.change_to_reverse();
    }
    else{
      this->motor.change_to_reverse();
      this->pid.SetControllerDirection(REVERSE);
      this->pid.SetMode(AUTOMATIC);
      this->pid.Compute();
      this->motor.set_pwm(this->power);
    }
  }

  return 0;

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

Rudder::~Rudder() {
	// TODO Auto-generated destructor stub
}
