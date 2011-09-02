#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <sstream>

class Logger;

class SerialBuffer
{
  Logger& log;
  std::ostringstream outstream;
  std::istringstream instream;

protected:
  static bool static_command_send(const std::vector<std::string>&);
  bool command_send(const std::vector<std::string>&);

public:
  SerialBuffer(Logger& _log): log(_log) {}
  void put(const std::string &);
  void put(char);
  bool available(void) const;
  char get(void);
  void flush(void);
  
  void setInput(const std::string&);
  void addCommandsTo(Dispatcher&);
};

#endif // __SERIAL_BUFFER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

