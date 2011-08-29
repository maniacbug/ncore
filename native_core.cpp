#include <WProgram.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

static struct timeval program_start;

extern "C" void __cxa_pure_virtual() { while (1); }

extern "C" {

void init(void)
{
  gettimeofday(&program_start,NULL);
  cout << "NCORE: " << setfill('0') << setw(6) << millis() << " started" << endl;
}

void delay(unsigned long ms)
{
  printf("NCORE: %06lu ",millis());
  printf("delay %lu\n",ms);
  usleep(ms*1000LU);
}

void digitalWrite(uint8_t pin,uint8_t level)
{
  printf("NCORE: %06lu ",millis());
  printf("pin %i: %s\n",pin,level?"HIGH":"LOW");
}

int digitalRead(uint8_t pin)
{
  int level = LOW;
  printf("NCORE: %06lu ",millis());
  printf("read pin %i: it's %s\n",pin,level?"HIGH":"LOW");

  return level;
}

void analogWrite(uint8_t pin,int level)
{
  printf("NCORE: %06lu ",millis());
  printf("pin %i: %i\n",pin,level);
}

int analogRead(uint8_t pin)
{
  int level = 0;
  printf("NCORE: %06lu ",millis());
  printf("read pin %i: it's %i\n",pin,level);
  return level;
}

void pinMode(uint8_t pin,uint8_t mode)
{
  printf("NCORE: %06lu ",millis());
  printf("pin %i: mode %s\n",pin,mode?"OUTPUT":"INPUT");
}

unsigned long millis(void)
{
  unsigned long result;
  struct timeval now;
  gettimeofday(&now,NULL);

  result = ( now.tv_sec - program_start.tv_sec ) * 1000LU ;
  result += ( now.tv_usec - program_start.tv_usec ) / 1000L ;
  
  //printf("NCORE: time is %lu\n",now.tv_sec,now.tv_usec,result);

  return result;
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout )
{
  printf("NCORE: %06lu ",millis());
  printf("pin %i: pulseIn 1000\n",pin);
  
  return 1000LU;
}
void delayMicroseconds(unsigned int us)
{
  printf("NCORE: %06lu ",millis());
  printf("delay %u us\n",us);
  usleep(us);
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

