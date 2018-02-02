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

// Initialize all 6 motors
Motor m1(5,28,29,3);
//Motor m2(1,2,3);
//Motor m3(1,2,3);
//Motor m4(1,2,3);
//Motor m5(1,2,3);
//Motor m6(1,2,3);

void setup(){
	Serial.begin(9600);
	m1.change_to_direct();
}

void loop(){
	Serial.println("I'm hereeee"); //upload complete indicator
	m1.set_pwm(200);
	m1.sense_current();
	delay(5000);
	//m1.stop_motor();
}
