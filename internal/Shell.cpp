#include <Shell.h>
#include <Dispatcher.h>
#include <Parser.h>
#include <iostream>
#include <stdexcept>
#include <readline/readline.h>

using namespace std;

void Shell::run(const Dispatcher& _commands)
{
  cout << "NCORE command shell" << endl;
  
  string command;
  do
  {
    char* input = readline("ncore$ ");
    if (input)
      command = string(input);
    else
      command = "quit";
    
    Parser parser(command);

    try
    {
      bool ok = true;
      if ( parser.size() )
	ok = _commands.execute(parser);
      if (!ok && command != "quit")
	cout << "Command not found: " << parser.at(0) << endl;
    }
    catch (runtime_error* e)
    {
      cout << "Error: " << e->what() << endl;
    }
  }
  while (command != "quit" );
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
