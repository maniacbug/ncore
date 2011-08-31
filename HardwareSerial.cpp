#include <sstream>
#include <string>

#include <WProgram.h>
#include "HardwareSerial.h"

HardwareSerial Serial;

static std::ostringstream outstream;
static std::istringstream instream;

void system_log(const std::string& s);

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long)
{
  system_log("Serial started.");
}

void HardwareSerial::end()
{
}

int HardwareSerial::available(void)
{
  return instream.good();
}

int HardwareSerial::peek(void)
{
  return 0;
}

int HardwareSerial::read(void)
{
  char result;
  instream.readsome( &result, 1 );
  return result;
}

void HardwareSerial::flush()
{
  system_log(outstream.str());
  outstream.str(std::string());
}

void HardwareSerial::write(uint8_t c)
{
  // Serial writes get put into a buffer, and then logged on CR
  
  if ( c == '\n' )
    flush();
  else
    outstream << c;
}

void serial_set(const std::string& str)
{
  instream.str(str);
}

void serial_clear(void)
{
  outstream.str(std::string());
  instream.str(std::string());
}

// vim:cin:ai:sts=2 sw=2
