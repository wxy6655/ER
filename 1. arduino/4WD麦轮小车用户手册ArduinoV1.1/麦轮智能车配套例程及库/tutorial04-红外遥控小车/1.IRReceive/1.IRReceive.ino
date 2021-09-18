/*
  Infrared receive - 红外接收
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#define SERIALDEBUG
#define RECV_PIN 2    //Sets the receive pin for the IR.

#include "IRremote.h"

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif

  irrecv.enableIRIn(); // Start the receiver
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif
  delay(100);
#ifdef SERIALDEBUG
  Serial.println("IR Recv Code:");
#endif
}

void loop() {
  if (irrecv.decode(&results)) {
#ifdef SERIALDEBUG
    Serial.println(results.value, HEX);
#endif
    irrecv.resume(); // Receive the next value
  }
  delay(50);
}
