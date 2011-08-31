#include <WProgram.h>
#line 1 "./Blink.pde"
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

static int level = LOW;
static int old_pin10 = LOW;

void setup()
{
  Serial.begin(57600);
  Serial.println("Blink.pde");
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
}

void loop()
{
  if ( digitalRead(10) )
  {
    if ( ! old_pin10 )
    {
      old_pin10 = HIGH;
      Serial.println("Starting...");
    }
    level ^= HIGH;
    digitalWrite(13, level);
    delay(1000);
  }
  else if ( old_pin10 )
  {
    old_pin10 = LOW;
    Serial.println("...Stopping");
  }
}
// vim:cin:ai:sts=2 sw=2
