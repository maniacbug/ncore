#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <vector>
#include <string>
#include <map>

class Commands
{
public:
  typedef bool (*worker)(const std::vector<std::string>&);
protected:

  std::map<std::string,worker> commandmap;

public:

  bool execute(const std::vector<std::string>&);
  bool add(const std::string&,worker);
};

#endif // __COMMANDS_H__

// vim:cin:ai:sts=2 sw=2 ft=cpp