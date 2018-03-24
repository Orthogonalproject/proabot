/*
 * proabot_rudder_seperate_pif_rudder.ino    Feb 2018
 * > 
 * > A newer version "boat_program_beta.ino" is 
 * > made to hide the implementation of PID and motor.
 * Author: Steven Hu
 * >Project Orthogonal  -Proabot
 */
#include "Arduino.h"
#include "Motor.h"
#include "Rudder.h"
#include <PID_v1.h>

double Input=0,Output=0,Setpoint=0;
double kp=2,ki=2,kd=1;
unsigned long last_time = 0;

// Initialize all 6 motors
Motor m1(5,28,29,3);
PID pid(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);
Rudder r1(15,m1);

const int command_p = 14;
//Motor m2(1,2,3);
//Motor m3(1,2,3);
//Motor m4(1,2,3);
//Motor m5(1,2,3);
//Motor m6(1,2,3);

void setup(){
	Serial.begin(9600);
	pinMode(command_p,INPUT);
	pinMode(45,OUTPUT);
	pinMode(47,OUTPUT);
	digitalWrite(45,HIGH);
	digitalWrite(47,HIGH);
	pid.SetSampleTime(1000);
	pid.SetOutputLimits(-255,255);
	pid.SetMode(AUTOMATIC);
}

void loop(){

	//if(millis()-last_time > 1000){

		//last_time = millis();

		Setpoint = analogRead(command_p);
		r1.set_command(Setpoint);
		Input = r1.get_position();

		Serial.print("Setpoint: ");
		Serial.println(Setpoint);
		Serial.print("Input: ");
		Serial.println(Input);
		Serial.print("Output: ");
		Serial.println(Output);
		Serial.println();
    if (abs(Setpoint-Input) < 15){
      r1.set_direction(STOP);
      pid.SetMode(MANUAL);
      Output = 0;
    }
    else {
      pid.SetMode(AUTOMATIC);
		  pid.Compute();
		  if (Output<0){
			  r1.set_direction(REVERSE);
			  r1.drive(-Output);
		  }
     
		  else{
			  r1.set_direction(DIRECT);
			  r1.drive(Output);
		  }
    }
		//Serial.print("power: ");
		//Serial.println(r1.get_power());
		Serial.println();
	}
//}
