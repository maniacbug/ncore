#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>

#include <stdarg.h>

#include <IDispatchable.h>

class Clock;
class Parser;

class Logger: public std::vector<std::string>, public IDispatchable
{
private:
  pthread_mutex_t mutex;
  const Clock* clock;
  unsigned long last_check;
  unsigned long rate_limit;
  static const unsigned long default_rate_limit = 500;
  int lines_remaining;
  char buffer[500];
  bool verbose; /**< Spew events to cerr as soon as they are logged */
protected:
  void throttle_output_rate(void);
  void add_buffer(const std::string& preamble);
  void add_message(const std::string& preamble,const std::string& message);

  std::string& getCommands(void) const
  {
    static std::string commands = "list log";
    return commands;
  }
  bool runCommand( const Parser& );
  bool command_list(const std::vector<std::string>&) const;
  bool command_log(const std::vector<std::string>&);
public:
  Logger(void);
  Logger(Clock&);
  ~Logger();
  void add(const std::string&,...);
  void internal(const std::string& module, const std::string& format,...);
  void sketch(const std::string& module, const std::string& format,...);
  void sketch_v(const std::string& module, const std::string& format, va_list ap);
  void clear(void);
  void setClock(Clock& _c)
  {
    clock = &_c;
  }
  void setRate(unsigned long _rate)
  {
    rate_limit = _rate;
  }
  int lines_contain(const std::string&) const;
  void setVerbose(bool _v)
  {
    verbose = _v;
  }
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
