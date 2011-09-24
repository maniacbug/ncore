#include <string>
#include <iostream>
#include <iomanip>

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>

#include <Logger.h>
#include <Pins.h>
#include <Clock.h>
#include <WProgram.h>

using namespace std;

extern Logger theLogger;
extern Pins thePins;
extern Clock theClock;

extern "C" {

void __cxa_pure_virtual() 
{
  theLogger.sketch("CORE","SYSTEM HALT -- cxa_pure_virtual");
  while (1) {} 
}

//
// Clock
//

void delay(unsigned long ms)
{
  theLogger.sketch("CLOK","delay %lu",ms);
  theClock.delay(ms);
}
unsigned long millis(void)
{
  return theClock.millis();
}
void delayMicroseconds(unsigned int us)
{
  theLogger.sketch("CLOK","delay %u us",us);
  theClock.delayMicroseconds(us);
}

//
// Pins
//

void digitalWrite(uint8_t pin,uint8_t level)
{
  thePins.digitalWrite(pin,level);
}

int digitalRead(uint8_t pin)
{
  return thePins.digitalRead(pin);
}

int analogRead(uint8_t pin)
{
  if ( pin >= A0 )
    pin -= A0;
  return thePins.analogRead(pin);
}

void pinMode(uint8_t pin,uint8_t mode)
{
  thePins.pinMode(pin,mode);
}

void attachInterrupt(uint8_t num, void (*fn)(void), int)
{
  thePins.attachInterrupt(num,fn);
}
void detachInterrupt(uint8_t num)
{
  thePins.detachInterrupt(num);
}

//
// Specialized functions
//

void printf_P(const char* format,...)
{
  std::string formatstr(format);

  // replace '%S' with '%s'
  size_t at = formatstr.find("%S");
  while ( at != string::npos )
  {
    formatstr.replace(at,2,"%s");
    at = formatstr.find("%S",at);
  }

  va_list args;
  va_start (args, format);
  //vprintf (formatstr.c_str(), args);
  theLogger.sketch_v("PRTF",formatstr.c_str(),args);
  
  va_end (args); 
}

//
// No-ops
//

unsigned long pulseIn(uint8_t pin, uint8_t /*state*/, unsigned long /*timeout*/ )
{
  theLogger.sketch("CORE","%i: pulseIn 1000",pin);
  
  return 1000LU;
}

void analogWrite(uint8_t pin,int level)
{
  theLogger.sketch("PINS","%i: %i",pin,level);
}

void fdevopen(int (*)(char, FILE*),int)
{
}

}

void tone(uint8_t pin, unsigned int what, unsigned long wait)
{
  theLogger.sketch("PINS","%i: tone %u",pin,what);
  delay(wait);
}

void noTone(uint8_t pin)
{
  theLogger.sketch("PINS","%i: noTone",pin);
}

// vim:cin:ai:sts=2 sw=2

