// STL includes
#include <iostream>
#include <stdexcept>
#include <sstream>
// C includes
// Library includes
#include <readline/readline.h>
#include <readline/history.h>
// Project includes
#include <Dispatcher.h>
#include <Parser.h>
#include <Clock.h>
#include <Shell.h>

/****************************************************************************/

using namespace std;

/****************************************************************************/

void Shell::run(const Dispatcher& _commands)
{
  run(_commands,Clock());
}

/****************************************************************************/

void Shell::run(const Dispatcher& _commands, const Clock& _clock)
{
  string command;
  do
  {
    ostringstream prompt;
    prompt << "ncore " << _clock.millis() << "$ ";
    char* input = readline(prompt.str().c_str());
    if (input)
    {
      command = string(input);
    }
    else
    {
      cout << endl;
      while ( remaining_schedule && remaining_schedule->size() )
      {
      }
      quit = true;
    }

    if ( input && command.size() )
    {
      bool valid = false;
      try
      {
        if (!_commands.execute_new(Parser(command)))
          cout << "Command failed: " << command << endl;
        else
          valid = true;
      }
      catch (runtime_error* e)
      {
        cout << "Error: " << e->what() << endl;
      }
      // Only add once we know it's a working command
      if (valid)
        add_history(input);
    }
  }
  while (!quit);
}

/****************************************************************************/

string& Shell::getCommands(void) const
{
  static std::string commands = "quit";
  return commands;
}

/****************************************************************************/

bool Shell::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "quit" )
  {
    quit = true;
    result = true;
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);
    if ( command == "quit" )
    {
      cout << "quit -- stop running and exit." << endl;
      result = true;
    }
  }

  return result;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
