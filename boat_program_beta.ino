#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <PID_v1.h>
#include "Rudder.h"
#include "Motor.h"

double data_to_send[4] = {0,50, 1,1};
//[0] := r1_position, [1] := r2_position, [2] := r1_current, [3] := r2_current
double received_data[2];   //received_data[0] = r1_setpoint, [1] = r2_setpoint

RF24 radio(9,53);
byte addresses[][6] = {"1Node","2Node"}; // ??

Rudder r1(15,5,28,29,3);
Rudder r2(1,1,1,1,1);

void setup()
{
	Serial.begin(9600);
	pinMode(47,OUTPUT); //give pot power
	digitalWrite(47, HIGH);

	//initiate the radio object
	radio.begin();
	//Set the transmit power to lowest available to prevent power supply related
	//issues
	radio.setPALevel(RF24_PA_MIN);

	//Set the speed of the transmission to the quickest available
	radio.setDataRate(RF24_2MBPS);

	//Use a channel unlikely to be used by Wifi, Microwave ovens etc
	radio.setChannel(124);

	//Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1,addresses[1]);

}

void update_data(){
	r1.update_setpoint(received_data[0]);
	r2.update_setpoint(received_data[1]);
	data_to_send[0] = r1.get_position();
	data_to_send[1] = 50;
	if (r1.get_stat()==HIGH_CURRENT)
		data_to_send[2] = 7;

}
void operate_rudder(Rudder & r){
	r.update_position();
	r.Compute_and_Drive();
	r.print();
	r.motor->sense_current();
}

void loop()
{
	radio.startListening();
	unsigned long start_wait_time = millis();
	while(!radio.available()){
		if(millis() - start_wait_time > 5){
			Serial.println("Nothing received!");
			return;
		}
	}

	radio.read(&received_data, sizeof(received_data));
	Serial.print("Received: ");
	Serial.println(received_data[0]);

	update_data(); //update setpoint and data_to_send

	radio.stopListening();

	operate_rudder(r1);
	//operate_rudder(r2);

	radio.write(&data_to_send, sizeof(data_to_send));
	Serial.print("Sent: ");
	Serial.println(data_to_send[0]);
}

