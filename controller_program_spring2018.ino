/*
 * controller_program_spring2018.ino
 *   Created: March 2018
 *   Updated: June 6, 2018
 *   Author: Steven, Asis
 *
 * >> Implemented:
 *   1.nrf	2.LCD screen
 *
 *   <Final version for Spring2018>
 *     The script implements the controller part of the radio-controlled
 *    proabot.
 */


#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include "LiquidCrystal_I2C.h"

#define HIGH_CURRENT 7    // The same encoding is used in "motor.h"


// Hardware configuration: Set up nRF24L01 radio on SPI bus (pins 10, 11, 12, 13) plus pins 7 & 8
RF24 radio(9, 53);  //CE, CSN pins
byte addresses[][6] = {"00001", "00002"};  //names of the two communications(2 directions)

//[0] setpoint_1; [1] setpoint_2
double data_to_send[2];
//[0] rudder_position_1; [1] rudder_position_2; [2] rudder_current_1; [3] rudder_current_2
double data_receive[4];

LiquidCrystal_I2C lcd(0x3F,16,2);

void radioSetup_controller()
{
	// Initiate the radio object
	radio.begin();
	// Set the transmit power to lowest available to prevent power supply related issues
	radio.setPALevel(RF24_PA_MIN);
	// Set the speed of the transmission to the quickest available
	radio.setDataRate(RF24_2MBPS);
	// Use a channel unlikely to be used by Wifi, Microwave ovens etc
	radio.setChannel(124);
	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(addresses[1]);
	radio.openReadingPipe(1, addresses[0]);
}


void setup() {
  Serial.begin(9600);

  radioSetup_controller();
  /* legacy code: These two pins were used to give potentiometer 5V
  pinMode(31,OUTPUT);
  pinMode(33,OUTPUT);
  digitalWrite(31,HIGH);
  digitalWrite(33,LOW);
  */



  lcd.begin();
  lcd.backlight();
  pinMode(A1, INPUT);// A1 will read the pot. to control rudder position.
  pinMode(A2, INPUT);
}

void show_info(){
	lcd.setCursor(0, 0);
	lcd.print(data_receive[1]);
	lcd.setCursor(7,0);
	lcd.print(data_receive[0]);
	if (data_receive[3]==HIGH_CURRENT){
		lcd.setCursor(6,0);
		lcd.print("H");
	}
	if(data_receive[2]==HIGH_CURRENT){
		lcd.setCursor(15,0);
		lcd.print("H");
	}

	lcd.setCursor(0,1);
	lcd.print(data_to_send[1]);
	lcd.setCursor(7,1);
	lcd.print(data_to_send[0]);

}

void update_setpoint(){
	data_to_send[0] = analogRead(A1); //r1_setpoint
	data_to_send[1] = analogRead(A2); //r2_setpoint
}

void radio_write(){
	// Ensure we have stopped listening (even if we're not) or we won't be able to transmit
	radio.stopListening();

	// Did we manage to SUCCESSFULLY transmit that (by getting an acknowledgement back from the other Arduino)?
	// Even we didn't we'll continue with the sketch, you never know, the radio fairies may help us
	if (!radio.write( &data_to_send, sizeof(data_to_send) )) {
		//Serial.println("No acknowledgement of transmission - receiving radio device connected?");
	}
	radio.write(&data_to_send,sizeof(data_to_send));
}

void radio_read(){
	// Now listen for a response
	radio.startListening();

	// But we won't listen for long, 200 milliseconds is enough
	unsigned long started_waiting_at = millis();

	// Loop here until we get indication that some data is ready for us to read (or we time out)
	while ( ! radio.available() ) {

    // Oh dear, no response received within our timescale
		if (millis() - started_waiting_at > 100 ) {
			Serial.println("No response received - timeout!");
			return;
		}
	}

	// Now read the data that is waiting for us in the nRF24L01's buffer

	radio.read( &data_receive, sizeof(data_receive) );
}

void monitor_print(){
	// Show user what we sent and what we got back
	Serial.print("Sent setpoint: ");
	Serial.print(data_to_send[0]);
	Serial.print("\t");
	Serial.println(data_to_send[1]);

	Serial.print("Received:\n");
	Serial.print("\tr1 position: ");
	Serial.println(data_receive[0]);
	Serial.print("\tr2 position: ");
	Serial.println(data_receive[1]);
	if (data_receive[2]==7)
		Serial.println("\t r1 stopped due to HIGH_CURRENT_ERROR");
	if (data_receive[3]==7)
		Serial.println("\t r2 stopped due to HIGH_CURRENT_ERROR");
}

void loop() {

	show_info();
	update_setpoint();

	radio_write();
	radio_read();

	monitor_print();
}
