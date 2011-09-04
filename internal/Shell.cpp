#include <iostream>
#include <stdexcept>
#include <sstream>

#include <readline/readline.h>
#include <readline/history.h>

#include <Shell.h>
#include <Dispatcher.h>
#include <Parser.h>
#include <Clock.h>

using namespace std;

void Shell::run(const Dispatcher& _commands)
{
  run(_commands,Clock());
}

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
      command = "quit";
   
    if ( command != "quit" && command != "")
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
  while (command != "quit" );
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
