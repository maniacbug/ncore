#include <iostream>
#include <stdexcept>
#include <sstream>
#include <readline/readline.h>
#include <pthread.h>

#include <Dispatcher.h>
#include <Parser.h>
#include <Logger.h>
#include <Pins.h>
#include <Shell.h>
#include <SketchThread.h>

using namespace std;

extern "C" void init(void);
extern void add_commands_to(Dispatcher&);

int main(void)
{
  Dispatcher dispatcher;
  
  init();

  add_commands_to(dispatcher);

  SketchThread thread;

  Shell().run(dispatcher);

  return 0;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
