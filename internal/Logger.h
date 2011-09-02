#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>

class Dispatcher;
class Clock;

class Logger: public std::vector<std::string>
{
private:
  pthread_mutex_t mutex;
  const Clock* clock;
  unsigned long last_check;
  int lines_remaining
protected:
  static void reset(void); /**< Reset state of statics. Only for testing! */
  void throttle_output_rate(void);
  
  static bool static_command_list(const std::vector<std::string>& _commands);
  bool command_list(const std::vector<std::string>&) const;
public:
  Logger(void);
  ~Logger();
  void add(const std::string&,...);
  void setClock(const Clock&);
  void clear(void);

  void addCommandsTo(Dispatcher&);
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
