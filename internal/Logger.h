#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>
#include <IDispatchable.h>

class Clock;
class Parser;

class Logger: public std::vector<std::string>, public IDispatchable
{
private:
  pthread_mutex_t mutex;
  const Clock* clock;
  unsigned long last_check;
  int lines_remaining;
protected:
  void throttle_output_rate(void);
  bool command_list(const std::vector<std::string>&) const;
public:
  Logger(void);
  ~Logger();
  void add(const std::string&,...);
  void setClock(const Clock&);
  void clear(void);
  
  std::string& getCommands(void) const { static std::string commands = "list"; return commands; }
  bool runCommand( const Parser& );
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
