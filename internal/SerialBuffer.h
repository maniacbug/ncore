#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <sstream>

#include <IDispatchable.h>

class Logger;
class Parser;

class SerialBuffer: public IDispatchable
{
  Logger& log;
  std::ostringstream outstream;
  std::istringstream instream;
  bool outstream_has_data;

protected:
  bool command_send(const std::vector<std::string>&);

public:
  SerialBuffer(Logger& _log): log(_log), outstream_has_data(false) {}
  void put(const std::string &);
  void put(char);
  bool available(void) const;
  char get(void);
  void flush(void);
  
  void setInput(const std::string&);

  std::string& getCommands(void) const { static std::string commands = "send"; return commands; }
  bool runCommand( const Parser& );
};

#endif // __SERIAL_BUFFER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

