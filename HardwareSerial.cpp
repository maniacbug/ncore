#include "HardwareSerial.h"

HardwareSerial Serial;
// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long)
{
  printf("NCORE: Serial started.\n");
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
  printf("FLUSH\n");
}

void HardwareSerial::write(uint8_t c)
{
  putchar(c);
}


