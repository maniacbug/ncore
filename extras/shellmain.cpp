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
#include <EepromInternal.h>
#include <SpiQueue.h>
#include <Scheduler.h>

using namespace std;

extern "C" void init(void);

Dispatcher theDispatcher;
Clock theClock;
Logger theLogger(theClock);
Pins thePins(theLogger);
SerialBuffer theSerialBuffer(theLogger);
Eeprom theEeprom(theLogger);
SpiQueue theSpiQueue(theLogger);
Scheduler theScheduler(theDispatcher,theLogger);

int main(void)
{
  // Announce
  cerr << "NCORE: Arduino Native Core" << endl << "Copyright (C) 2011 maniacbug@ymail.com GPLv2" << endl << endl;

  // Command Shell
  Shell shell(&theScheduler);

  // Add commands for all of the objects that can dispatch commands
  theDispatcher.add(&theLogger);
  theDispatcher.add(&thePins);
  theDispatcher.add(&theSerialBuffer);
  theDispatcher.add(&theEeprom);
  theDispatcher.add(&theSpiQueue);
  theDispatcher.add(&theScheduler);
  theDispatcher.add(&shell);
 
  // Announce to the log
  theLogger.internal("CORE","Started");

  // Launch the sketch in its own thread
  SketchThread thread;

  // Launch the scheduler
  SketchThread scheduler_thread(1);
  scheduler_thread.startCustom(Scheduler::handler_thread_main,&theScheduler);

  // Operate the shell
  shell.run(theDispatcher,theClock);

  return 0;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
