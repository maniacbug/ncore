
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

// STL includes
#include <queue>
#include <string>

// C includes

// Library includes
#include <semaphore.h>

// Project includes
#include <Clock.h>
#include <IDispatchable.h>
#include <IContainer.h>

/**
 * An object which can be scheduled
 */

struct SchedulableObject
{
  unsigned long trigger_at;
  std::string commands;
  SchedulableObject( unsigned long _t, const std::string& _p ): trigger_at(_t), commands(_p) {}
  // lesser value means later in time
  bool operator<(const SchedulableObject& _rhs) const
  {
    return trigger_at > _rhs.trigger_at;
  }
};

class Dispatcher;
class Logger;

/**
 * Allows for the scheduling of commands at a time in the future
 */

class Scheduler: public IDispatchable, public IContainer
{
private:
  std::priority_queue<SchedulableObject> object_q;
  Dispatcher& dispatch;
  Logger& logger;
  Clock clock; // TODO: Need to pass this in!!
  sem_t sem;
  bool done;

// left public for tests
public:  
//protected:
  void runonce(void);

protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );
  bool command_at(const std::vector<std::string>&);
public:
  Scheduler(Dispatcher& _d, Logger& _l);
  virtual ~Scheduler();
  void add(unsigned long trigger_at, const std::string& commands );
  size_t size(void) const;
  void clear(void);

  static void handler_thread_main(void*);
};

#endif // __SCHEDULER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
