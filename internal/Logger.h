#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>

class Dispatcher;

class Logger: public std::vector<std::string>
{
protected:
  static void reset(void); /**< Reset state of statics. Only for testing! */
  static bool static_command_list(const std::vector<std::string>& _commands);
  bool command_list(const std::vector<std::string>&) const;
public:
  void add(const std::string&,...);

  void addCommandsTo(Dispatcher&);
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
