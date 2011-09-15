
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __IDISPATCHABLE_H__
#define __IDISPATCHABLE_H__

#include <string>

class Parser;

/**
 * Interface for objects which can handler dispatcher commands
 */

class IDispatchable
{
public:
  virtual std::string& getCommands(void) const = 0;
  virtual bool runCommand( const Parser& ) = 0;
};

#endif // __IDISPATCHABLE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
