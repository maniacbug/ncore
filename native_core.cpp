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

static Logger logger;
static Pins pins;
static Clock theclock;

void add_commands_to(Dispatcher& d)
{
  pins.addCommandsTo(d);
  logger.addCommandsTo(d);
}

void system_log(const string& s)
{
  logger.add(s);
}

extern "C" {

void init(void)
{
  logger.setClock(theclock);
  logger.add("started");
}

//
// Clock
//

void delay(unsigned long ms)
{
  logger.add("delay %lu",ms);
  theclock.delay(ms);
}
unsigned long millis(void)
{
  return theclock.millis();
}
void delayMicroseconds(unsigned int us)
{
  logger.add("delay %u us\n",us);
  theclock.delayMicroseconds(us);
}

//
// Pins
//

void digitalWrite(uint8_t pin,uint8_t level)
{
  logger.add("pin %i: %s",pin,level?"HIGH":"LOW");
  pins.digitalWrite(pin,level);
}

int digitalRead(uint8_t pin)
{
  int level = LOW;
  level = pins.digitalRead(pin);
  //logger.add("read pin %i: it's %s",pin,level?"HIGH":"LOW");

  return level;
}

int analogRead(uint8_t pin)
{
  int level = pins.analogRead(pin);
  logger.add("read pin %i: it's %i\n",pin,level);
  return level;
}

void pinMode(uint8_t pin,uint8_t mode)
{
  logger.add("pin %i: mode %s",pin,mode?"OUTPUT":"INPUT");
  pins.pinMode(pin,mode);
}

void attachInterrupt(uint8_t num, void (*fn)(void), int)
{
  logger.add("attach irq %u\n",num);
  pins.attachInterrupt(num,fn);
}
void detachInterrupt(uint8_t num)
{
  logger.add("detach irq %u\n",num);
  pins.detachInterrupt(num);
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
  logger.add("pin %i: pulseIn 1000\n",pin);
  
  return 1000LU;
}

void analogWrite(uint8_t pin,int level)
{
  logger.add("pin %i: %i\n",pin,level);
}

void fdevopen(int (*)(char, FILE*),int)
{
}

}
// vim:cin:ai:sts=2 sw=2

