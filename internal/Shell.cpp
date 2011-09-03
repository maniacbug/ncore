#include <iostream>
#include <stdexcept>
#include <sstream>

#include <readline/readline.h>

#include <Shell.h>
#include <Dispatcher.h>
#include <Parser.h>
#include <Clock.h>

using namespace std;

void Shell::run(const Dispatcher& _commands) //, const Clock& _clock)
{
  Clock _clock;

  cout << "NCORE command shell" << endl;
  
  string command;
  do
  {
    ostringstream prompt;
    prompt << "ncore " << _clock.millis() << "$ ";
    char* input = readline(prompt.str().c_str());
    if (input)
      command = string(input);
    else
      command = "quit";
   
    if ( command != "quit" && command != "")
    {
      try
      {
	if (!_commands.execute_new(Parser(command)))
	  cout << "Command failed: " << command << endl;
      }
      catch (runtime_error* e)
      {
	cout << "Error: " << e->what() << endl;
      }
    }
  }
  while (command != "quit" );
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
