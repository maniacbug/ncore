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

static Logger* logger = NULL;
static Pins* pins = NULL;

static Clock theclock;

void init(Pins& _p,Logger& _l)
{
  pins = &_p;
  logger = &_l;
  logger->add("started"); 
}

extern "C" {

void init(void)
{
  logger = new Logger;
  logger->add("started"); 
}

void delay(unsigned long ms)
{
  logger->add("delay %lu",ms);
  theclock.delay(ms);
}

void digitalWrite(uint8_t pin,uint8_t level)
{
  logger->add("pin %i: %s",pin,level?"HIGH":"LOW");
  if ( pins )
    pins->digitalWrite(pin,level);
}

int digitalRead(uint8_t pin)
{
  int level = LOW;
  if ( pins )
    level = pins->digitalRead(pin);
  //logger->add("read pin %i: it's %s",pin,level?"HIGH":"LOW");

  return level;
}

void analogWrite(uint8_t pin,int level)
{
  logger->add("pin %i: %i\n",pin,level);
}

int analogRead(uint8_t pin)
{
  int level = 0;
  logger->add("read pin %i: it's %i\n",pin,level);
  if ( pins )
    level = pins->analogRead(pin);
  return level;
}

void pinMode(uint8_t pin,uint8_t mode)
{
  logger->add("pin %i: mode %s",pin,mode?"OUTPUT":"INPUT");
  if ( pins )
    pins->pinMode(pin,mode);
}

unsigned long millis(void)
{
  return theclock.millis();
}

unsigned long pulseIn(uint8_t pin, uint8_t /*state*/, unsigned long /*timeout*/ )
{
  printf("NCORE: %06lu ",millis());
  printf("pin %i: pulseIn 1000\n",pin);
  
  return 1000LU;
}
void delayMicroseconds(unsigned int us)
{
  logger->add("delay %u us\n",us);
  theclock.delayMicroseconds(us);
}

void fdevopen(int (*)(char, FILE*),int)
{
}

void attachInterrupt(uint8_t num, void (*)(void), int)
{
  printf("NCORE: %06lu ",millis());
  printf("attach irq %u\n",num);
}
void detachInterrupt(uint8_t num)
{
  printf("NCORE: %06lu ",millis());
  printf("detach irq %u\n",num);
}

void printf_P(const char* format,...)
{
  string formatstr(format);

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

}
// vim:cin:ai:sts=2 sw=2

