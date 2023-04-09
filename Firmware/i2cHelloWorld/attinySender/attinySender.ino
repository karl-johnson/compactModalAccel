// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define LED_PIN PIN_PA4

void setup() {
  pinMode(LED_PIN, OUTPUT);
  //Wire.pins(PIN_PB1, PIN_PB0);
  Wire.onRequest(requestEvent); // register event
  Wire.begin(0x54);                
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  digitalWrite(LED_PIN, HIGH);
  Wire.write('b'); // respond with message of 6 bytes
  delay(50);
  digitalWrite(LED_PIN, LOW);
  // as expected by master
}
