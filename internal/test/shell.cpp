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
#include <SerialBuffer.h>
#include <Clock.h>

using namespace std;

Dispatcher theDispatcher;
Logger theLogger;
Pins thePins;
Clock theClock;
SerialBuffer theSerialBuffer(theLogger);

extern "C" void init(void);

int main(void)
{
  theLogger.addCommandsTo(theDispatcher);
  thePins.addCommandsTo(theDispatcher);
  theSerialBuffer.addCommandsTo(theDispatcher);
  
  init();

  SketchThread thread;

  Shell().run(theDispatcher);

  return 0;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
