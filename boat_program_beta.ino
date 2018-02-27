#include "Arduino.h"
#include "Wire.h"
#include <PID_v1.h>
#include "Rudder.h"
#include "Motor.h"

Rudder r1(15,5,28,29,3);
Rudder r2(1,1,1,1,1);

void setup()
{
	Serial.begin(9600);
	pinMode(47,OUTPUT); //give pot power
	digitalWrite(47, HIGH);
}

void operate_rudder(Rudder & r){
	r.update_position();
	r.update_setpoint(0);  //argument should be data[index]
	r.Compute_and_Drive();
	r.print();
	r.motor->sense_current();
}

void loop()
{
	operate_rudder(r1);
	operate_rudder(r2);

}

