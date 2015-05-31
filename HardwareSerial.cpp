#include <sstream>
#include <string>

#include <Logger.h>
#include <SerialBuffer.h>

#include <Arduino.h>
#include "HardwareSerial.h"

HardwareSerial Serial;
HardwareSerial Serial1;

extern Logger theLogger;
extern SerialBuffer theSerialBuffer;

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long)
{
  theLogger.sketch("CORE","Serial started.");
}

void HardwareSerial::end()
{
}

int HardwareSerial::available(void)
{
  return theSerialBuffer.available();
}

int HardwareSerial::peek(void)
{
  return theSerialBuffer.peek();
}

int HardwareSerial::read(void)
{
  return theSerialBuffer.get();
}

void HardwareSerial::flush()
{
  // theSerialBuffer.flush();
}

size_t HardwareSerial::write(uint8_t c)
{
  return theSerialBuffer.put(c);
}

// vim:cin:ai:sts=2 sw=2
