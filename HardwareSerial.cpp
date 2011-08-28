#include <WProgram.h>
#include "HardwareSerial.h"

HardwareSerial Serial;
// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long)
{
  printf("NCORE: %06lu ",millis());
  printf("Serial started.\n");
}

void HardwareSerial::end()
{
}

int HardwareSerial::available(void)
{
  return 0;
}

int HardwareSerial::peek(void)
{
  return 0;
}

int HardwareSerial::read(void)
{
  return getchar();
}

void HardwareSerial::flush()
{
  printf("NCORE: %06lu ",millis());
  printf("Serial flushed.\n");
}

void HardwareSerial::write(uint8_t c)
{
  putchar(c);
}


