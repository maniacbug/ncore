#include <WProgram.h>
#line 1 "./Blink.pde"
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

static int level = LOW;

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
}

void loop() {
  if ( digitalRead(10) )
{
  level ^= HIGH;
  digitalWrite(13, level);  
  delay(1000);             
}
}
