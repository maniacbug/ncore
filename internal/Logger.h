#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>
#include <IDispatchable.h>

#include <stdarg.h>

class Clock;
class Parser;

class Logger: public std::vector<std::string>, public IDispatchable
{
private:
  pthread_mutex_t mutex;
  const Clock* clock;
  unsigned long last_check;
  int lines_remaining;
  char buffer[500];
protected:
  void throttle_output_rate(void);
  bool command_list(const std::vector<std::string>&) const;
  void add_buffer(const std::string& preamble);
  void add_message(const std::string& preamble,const std::string& message);
public:
  Logger(void);
  Logger(Clock&);
  ~Logger();
  void add(const std::string&,...);
  void internal(const std::string& module, const std::string& format,...);
  void sketch(const std::string& module, const std::string& format,...);
  void clear(void);
  
  std::string& getCommands(void) const { static std::string commands = "list"; return commands; }
  bool runCommand( const Parser& );
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
