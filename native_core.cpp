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

extern "C" void __cxa_pure_virtual() { while (1); }

extern Logger theLogger;
extern Pins thePins;
extern Clock theClock;

extern "C" {

void init(void)
{
  theLogger.setClock(theClock);
  theLogger.add("started");
}

//
// Clock
//

void delay(unsigned long ms)
{
  theLogger.add("delay %lu",ms);
  theClock.delay(ms);
}
unsigned long millis(void)
{
  return theClock.millis();
}
void delayMicroseconds(unsigned int us)
{
  theLogger.add("delay %u us\n",us);
  theClock.delayMicroseconds(us);
}

//
// Pins
//

void digitalWrite(uint8_t pin,uint8_t level)
{
  theLogger.add("pin %i: %s",pin,level?"HIGH":"LOW");
  thePins.digitalWrite(pin,level);
}

int digitalRead(uint8_t pin)
{
  int level = LOW;
  level = thePins.digitalRead(pin);
  //theLogger.add("read pin %i: it's %s",pin,level?"HIGH":"LOW");

  return level;
}

int analogRead(uint8_t pin)
{
  int level = thePins.analogRead(pin);
  theLogger.add("read pin %i: it's %i\n",pin,level);
  return level;
}

void pinMode(uint8_t pin,uint8_t mode)
{
  theLogger.add("pin %i: mode %s",pin,mode?"OUTPUT":"INPUT");
  thePins.pinMode(pin,mode);
}

void attachInterrupt(uint8_t num, void (*fn)(void), int)
{
  theLogger.add("attach irq %u\n",num);
  thePins.attachInterrupt(num,fn);
}
void detachInterrupt(uint8_t num)
{
  theLogger.add("detach irq %u\n",num);
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
  vprintf (formatstr.c_str(), args);
  va_end (args); 
}

//
// No-ops
//

unsigned long pulseIn(uint8_t pin, uint8_t /*state*/, unsigned long /*timeout*/ )
{
  theLogger.add("pin %i: pulseIn 1000\n",pin);
  
  return 1000LU;
}

void analogWrite(uint8_t pin,int level)
{
  theLogger.add("pin %i: %i\n",pin,level);
}

void fdevopen(int (*)(char, FILE*),int)
{
}

}
// vim:cin:ai:sts=2 sw=2

