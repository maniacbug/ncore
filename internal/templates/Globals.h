#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// STL includes
// C includes
// Library includes
#include <Dispatcher.h>
#include <Parser.h>
#include <Logger.h>
#include <Pins.h>
#include <Shell.h>
#include <SketchThread.h>
#include <SerialBuffer.h>
#include <Clock.h>
#include <Eeprom.h>
#include <SpiQueue.h>
#include <Scheduler.h>
// Project includes

extern Dispatcher theDispatcher;
extern Clock theClock;
extern Logger theLogger;
extern Pins thePins;
extern SerialBuffer theSerialBuffer;
extern Eeprom theEeprom;
extern SpiQueue theSpiQueue;
extern Scheduler theScheduler;

#endif // __GLOBAL_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
