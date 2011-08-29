#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>

class Logger: public std::vector<std::string>
{
public:
  void add(const std::string&);
};

#endif // __LOGGER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
