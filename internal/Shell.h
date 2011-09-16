
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include <IDispatchable.h>
#include <IContainer.h>

class Dispatcher;
class Clock;

/**
 * Encapsulates the shell loop.  Gets new commands from the user, sends to dispatcher.
 */

class Shell: public IDispatchable
{
private:
  bool quit;
  IContainer* remaining_schedule;
protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );
public:
  Shell(IContainer* _remaining_schedule = NULL): quit(false), remaining_schedule(_remaining_schedule) {}
  void run(const Dispatcher& commands);
  void run(const Dispatcher& commands, const Clock& clock);
};

#endif // __SHELL_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

