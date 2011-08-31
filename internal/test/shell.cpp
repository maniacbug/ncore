#include <iostream>
#include <stdexcept>
#include <sstream>
#include <readline/readline.h>
#include <pthread.h>

#include <Dispatcher.h>
#include <Parser.h>
#include <Logger.h>
#include <Pins.h>
#include <wiring.h>

using namespace std;

extern void add_commands_to(Dispatcher&);

Dispatcher dispatcher;

static void* sketch_main(void*)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

  setup();

  while(1)
    loop();

  return NULL;
}

int main(void)
{
  init();

  add_commands_to(dispatcher);

  pthread_t sketch_thread;
  pthread_create( &sketch_thread, NULL, sketch_main, NULL );

  string command;
  do
  {
    ostringstream prompt;
    prompt << "ncore " << millis() << "$ ";
    char* input = readline(prompt.str().c_str());
    if (input)
      command = string(input);
    else
      command = "quit";

    Parser parser(command);

    try
    {
      bool ok = true;
      if (parser.size() )
        ok = dispatcher.execute(parser);
      if (!ok && command != "quit")
        cout << "Command not found: " << parser.at(0) << endl;
    }
    catch (runtime_error* e)
    {
      cout << "Error: " << e->what() << endl;
    }
  }
  while (command != "quit" );

  pthread_cancel( sketch_thread );
  pthread_join( sketch_thread, NULL );

  return 0;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
