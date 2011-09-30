// STL includes
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
// C includes
// Library includes
// Project includes
#include <Parser.h>
#include <IDispatchable.h>
#include <Dispatcher.h>

using namespace std;

/****************************************************************************/

void Dispatcher::call_help(const objectmap_t::value_type& _item)
{
  Parser help;
  help.resize(2);
  help.at(0) = "help";
  help.at(1) = _item.first;
  (_item.second)->runCommand(help);
}

/****************************************************************************/

bool Dispatcher::execute_new(const Parser& _commands) const
{
  if ( ! _commands.size() )
    throw new runtime_error("Command string empty");

  string command = _commands.at(0);

  // Redirect 'help <xxx>' to the appropriate command handler.
  if ( command == "help" )
  {
    if ( _commands.size() < 2 )
    {
      // Send a 'help' to every single command on the list
      for_each(objectmap.begin(),objectmap.end(),call_help);
      return true;
    }

    command = _commands.at(1);
  }

  if ( ! objectmap.count( command ) )
    throw new runtime_error("Command not found");

  return objectmap.at(command)->runCommand(_commands);
}

/****************************************************************************/

bool Dispatcher::execute(const std::string& _str) const
{
  return execute_new(Parser(_str));
}

/****************************************************************************/

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

/****************************************************************************/

void Dispatcher::clear(void)
{
  objectmap.clear();
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
