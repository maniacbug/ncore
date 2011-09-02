#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <sstream>

class Logger;

class SerialBuffer
{
  Logger& log;
  std::ostringstream outstream;
public:
  SerialBuffer(Logger& _log): log(_log) {}
  void put(const std::string &);
};

#endif // __SERIAL_BUFFER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

