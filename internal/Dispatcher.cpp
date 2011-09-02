#include <vector>
#include <string>
#include <iostream>

#include <Parser.h>
#include <Dispatcher.h>

using namespace std;

bool Dispatcher::execute(const std::vector<std::string>& _commands) const
{
  bool result = false;
  const string& command = ( _commands.size() > 0 ) ? _commands.at(0) : string();

  if ( commandmap.count( command ) )
  {
    result = commandmap.at(command)(_commands);
  }

  return result;
}

bool Dispatcher::execute(const std::string& _str) const
{
  return execute(Parser(_str));
}

bool Dispatcher::add(const std::string& _command, worker _fn)
{
  bool result = false;

  if ( ! commandmap.count( _command ) )
  {
    commandmap[_command] = _fn;
    result = true;
  }

  return result;
}

void Dispatcher::clear(void)
{
  commandmap.clear();
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
