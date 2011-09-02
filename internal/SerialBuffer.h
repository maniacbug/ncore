#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <sstream>

class Logger;

class SerialBuffer
{
  Logger& log;
  std::ostringstream outstream;
  std::istringstream instream;
public:
  SerialBuffer(Logger& _log): log(_log) {}
  void put(const std::string &);
  void put(char);
  bool available(void) const;
  void setInput(const std::string&);
  char get(void);
};

#endif // __SERIAL_BUFFER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

