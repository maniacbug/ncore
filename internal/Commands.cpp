#include <Commands.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool Commands::execute(const std::vector<std::string>& _commands)
{
  bool result = false;
  const string& command = ( _commands.size() > 0 ) ? _commands.at(0) : string();

  if ( commandmap.count( command ) )
  {
    result = commandmap.at(command)(_commands);
  }

  return result;
}

bool Commands::add(const std::string& _command, worker _fn)
{
  bool result = false;

  if ( ! commandmap.count( _command ) )
  {
    commandmap[_command] = _fn;
    result = true;
  }

  return result;
}

void Commands::clear(void)
{
  commandmap.clear();
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
