#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>

class Commands;

class Logger: public std::vector<std::string>
{
protected:
  static bool static_command_list(const std::vector<std::string>& _commands);
public:
  void add(const std::string&,...);
  
  void addCommandsTo(Commands&);
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
