#include <WProgram.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

static struct timeval program_start;

extern "C" void __cxa_pure_virtual() { while (1); }

extern "C" {

void init(void)
{
  gettimeofday(&program_start,NULL);
  printf("NCORE: %06lu ",millis());
  printf("started\n");
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
  printf("NCORE: read pin %i: it's %s\n",pin,level?"HIGH":"LOW");

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

}
