/*
 * boat_program_spring2018.ino
 *   Created: March 2018
 *   Updated: June 6, 2018
 *   Author: Steven, Asis
 *
 * >> Implemented:
 *   1.nrf	2.PID	3. Two rudders
 *
 *   <Final version for Spring2018>
 *    The script uses PID controller to drive two rudders on the
 *    radio-controlled proabot.
 */

#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <PID_v1.h>
#include "Rudder.h"
#include "Motor.h"

/* [0] := r1_position, [1] := r2_position,
 * [2] := r1_current_sensing_status, [3] := r2_current_sensing_status
 *  current_sensing_condition can either be 0 or HIGH_CURRENT(=7 defined in
 *  motor.h).
 */
double data_to_send[4] = {0,0, 1,1};

//received_data[0] = r1_setpoint, [1] = r2_setpoint
double received_data[2];

RF24 radio(9,53);  //CE, CSN pins
byte addresses[][6] = {"00001","00002"};
  //names of the two communications(2 directions)

/* construct two rudder objects:
 *   Rudder(position_p, pwm_p, in1_p, in2_p, current_p)
 */
Rudder r1(15,5,28,29,3);
Rudder r2(8,4,26,27,2);

void radioSetup()
{
	//initiate the radio object
	radio.begin();

	/*
	 * Set the transmit power to lowest available to prevent power supply related
	 * If using a higer level, a bypass capacitor across GND and 3.3V should be
	 * used to smooth out the voltage.
	 */
	radio.setPALevel(RF24_PA_MIN);

	//Set the speed of the transmission to the quickest available
	radio.setDataRate(RF24_2MBPS);

	//Use a channel unlikely to be used by Wifi, Microwave ovens etc
	radio.setChannel(124);

	//Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1,addresses[1]);
}

/*
 * OutputLimits and Tunings were chosen for our application only.
 * set_limits(int,int) sets the position limits for the two rudders.
 */
void pidSetup()
{
	r1.pid->SetOutputLimits(-135,135);
	r2.pid->SetOutputLimits(-185,185);
	r1.pid->SetTunings(4,1,2.5);
	r2.pid->SetTunings(4,1,3);
	r1.set_limits(422,610);
	r2.set_limits(279,575);
}

void setup()
{
	Serial.begin(9600);

	radioSetup();
	pidSetup();
}

/*
 * update_data() 1. passes the received setpoints to the two rudder objects.
 * 2. get the positions of the rudders and update the data_to_send array.
 * 3. check current sensing result. If HIGH_CURRENT, update data_to_send
 */
void update_data(){
	r1.update_setpoint(received_data[0]);
	r2.update_setpoint(received_data[1]);
	data_to_send[0] = r1.get_position();
	data_to_send[1] = r2.get_position();
	if (r1.get_stat()==HIGH_CURRENT)
		data_to_send[2] = HIGH_CURRENT;
	if (r2.get_stat() == HIGH_CURRENT)
		data_to_send[3] = HIGH_CURRENT;

}
void operate_rudder(Rudder & r){
	r.update_position();
	r.Compute_and_Drive();
	r.print();
	r.motor->sense_current();
}

void loop()
{
	//start listening to the radio. wait until it receives a signal or timeout
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
	Serial.print(received_data[0]);
	Serial.print("\t");
	Serial.println(received_data[1]);

	update_data(); //update setpoint and data_to_send

	radio.stopListening();

	operate_rudder(r1);
	operate_rudder(r2);

	//send the rudder positions and current_sensing results
	radio.write(&data_to_send, sizeof(data_to_send));
	Serial.print("Sent: ");
	Serial.println(data_to_send[0]);
}

