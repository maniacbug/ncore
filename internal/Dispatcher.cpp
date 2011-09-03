#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

#include <Parser.h>
#include <Dispatcher.h>
#include <IDispatchable.h>

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

bool Dispatcher::execute_new(const Parser& _commands) const
{
  if ( ! _commands.size() )
    throw new runtime_error("Command string empty");

  const string& command = _commands.at(0);

  if ( ! objectmap.count( command ) )
    throw new runtime_error("Command not found");
  
  return objectmap.at(command)->runCommand(_commands);
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
  
bool Dispatcher::add(IDispatchable* obj)
{
  if ( ! obj )
    throw new runtime_error("Object is NULL");

  Parser list(obj->getCommands());

  if ( ! list.size() )
    throw new runtime_error("Object has no commands");

  Parser::const_iterator it = list.begin();
  while ( it != list.end() )
  {
    if ( objectmap.count( *it ) )
      throw new runtime_error("Command already registered");
    
    objectmap[*it++] = obj;
  }  
  return true;
}

void Dispatcher::clear(void)
{
  commandmap.clear();
  objectmap.clear();
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
