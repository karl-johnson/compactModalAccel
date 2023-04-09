#include <Wire.h>

#define MySerial Serial

//#define ENABLE_RECEIVE
#define ENABLE_REQUEST

#define LED_PIN PIN_PA4

void setup() {
  //Wire.pins(PIN_PB1, PIN_PB0);
  //pinMode(LED_PIN, OUTPUT);
  #ifdef ENABLE_RECEIVE
  Wire.onReceive(receiveDataWire);  // give the Wire library the name of the function
  //                                   that will be called on a master write event
  #endif
  #ifdef ENABLE_REQUEST
  Wire.onRequest(transmitDataWire); // same as above, but master read event
  #endif
  Wire.begin(0x54);                 // join i2c bus with address 0x54
}

void loop() {
  //digitalWrite(LED_PIN, HIGH);
  //delay(100);
  //digitalWrite(LED_PIN, LOW);
  delay(100);
}

#ifdef ENABLE_RECEIVE
void receiveDataWire(int16_t numBytes) {      // the Wire API tells us how many bytes
  for (uint8_t i = 0; i < numBytes; i++) {    // were received so we can for loop for that
    char c = Wire.read();                     // amount and read the received data
    MySerial.write(c);                         // to print it to the Serial Monitor
  }
}
#endif

#ifdef ENABLE_REQUEST
void transmitDataWire() {
  #if !defined(MILLIS_USE_TIMERNONE)
  uint32_t ms = millis();
  #else
  uint32_t ms = 123456789UL; // placeholder - there's no millis to send because it's disabled.
  #endif
  Wire.write((uint8_t) ms);
  Wire.write((uint8_t)(ms >> 8));
  Wire.write((uint8_t)(ms >> 16));
  Wire.write((uint8_t)(ms >> 24));
}
#endif
