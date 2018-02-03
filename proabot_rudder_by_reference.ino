/*
 * motor_test.ino
 * > A test file for the motor control board and
 * > Class Motor.
 *
 * Author: Steven Hu
 * >Project Orthogonal  -Proabot
 */
#include "Arduino.h"
#include "Motor.h"
#include "Rudder.h"
#include <PID_v1.h>

double Input,Output,Setpoint;
double kp=2,ki=5,kd=1;
// Initialize all 6 motors
Motor m1(5,28,29,3);
PID pid(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);
Rudder r1(15,m1,pid);

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

}

void loop(){
	Setpoint = analogRead(command_p);
	Input = r1.get_position();
	Output = r1.get_power();

	r1.drive();
	int stat = Serial.print("stat: ");
	Serial.println(stat);
	Serial.print("position: ");
	Serial.println(r1.get_position());
	Serial.print("power: ");
	Serial.println(r1.get_power());
}
