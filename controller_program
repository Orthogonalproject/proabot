#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>

// This is just the way the RF24 library works:
// Hardware configuration: Set up nRF24L01 radio on SPI bus (pins 10, 11, 12, 13) plus pins 7 & 8
RF24 radio(9, 53);

byte addresses[][6] = {"1Node", "2Node"};
double data_to_send[2]; // [0] setpoint_1; [1] setpoint_2 
double data_receive[4]; // [0] rudder_position_1; [1] rudder_position_2

// -----------------------------------------------------------------------------
// SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
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

  // Random number seeding (we're going to be sending a single random number)
  //randomSeed(analogRead(A0));
  
  pinMode(A1, INPUT);// this will read the pot. to control rudder possition. 
}

// -----------------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------------
void loop() {
  data_to_send[0] = analogRead(A1); //setpoint from pot
      
  // Ensure we have stopped listening (even if we're not) or we won't be able to transmit
  radio.stopListening(); 
  
  // Did we manage to SUCCESSFULLY transmit that (by getting an acknowledgement back from the other Arduino)?
  // Even we didn't we'll continue with the sketch, you never know, the radio fairies may help us
  if (!radio.write( &data_to_send, sizeof(data_to_send) )) {
    //Serial.println("No acknowledgement of transmission - receiving radio device connected?");    
  }
  radio.write(&data_to_send,sizeof(data_to_send));
  
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
  //String dataRx;
  radio.read( &data_receive, sizeof(data_receive) );
  
  // Show user what we sent and what we got back
  Serial.print("Sent setpoint: ");
  Serial.println(data_to_send[0]);
  Serial.print("Received:\n");
  Serial.print("\tr1 position: ");
  Serial.println(data_receive[0]);
  Serial.print("\tr2 position: ");
  Serial.println(data_receive[1]);
  if (data_receive[3]==7)
    Serial.println("\t r1 stopped due to HIGH_CURRENT_ERROR");
  
}
