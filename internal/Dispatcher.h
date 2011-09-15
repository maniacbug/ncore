
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include <vector>
#include <string>
#include <map>

class IDispatchable;
class Parser;

/**
 * Dispatches commands to registered objects
 */
class Dispatcher
{
protected:
  /**
   * Map of commands to dispatchable objects
   */
  typedef std::map<std::string,IDispatchable*> objectmap_t;
  objectmap_t objectmap;

public:
  /**
   * Executes the worker associated with the first token in @p commands
   *
   * @param commands Tokenized command and parameters
   *
   * @retval true Worker function found and returned true
   * @retval false Worker function not found or returned false
   */
  bool execute(const std::string& commands) const;
  bool execute_new(const Parser& commands) const;

  bool add(IDispatchable*);

  /**
   * Empties the entire map
   */
  void clear(void);

  size_t size(void) const
  {
    return objectmap.size();
  }
};

#endif // __DISPATCHER_H__

// vim:cin:ai:sts=2 sw=2 ft=cpp
