
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include <string>
#include <map>

class IDispatchable;
class Parser;

/**
 * Dispatches commands to registered objects
 */
class Dispatcher
{
private:
  typedef std::map<std::string,IDispatchable*> objectmap_t; /**< Type used by internal map. */ 
  objectmap_t objectmap; /**< Map of commands to dispatchable objects */

public:
  /**
   * Executes the object associated with the first token in @p commands
   *
   * @param commands Simple string of space-separated command and parameters
   *
   * @retval true Worker function found and returned true
   * @retval false Worker function not found or returned false
   */
  bool execute(const std::string& commands) const;
  
  /**
   * Executes the object associated with the first token in @p commands
   *
   * @param commands Tokenized command and parameters
   *
   * @retval true Worker function found and returned true
   * @retval false Worker function not found or returned false
   */
  bool execute_new(const Parser& commands) const;

  /**
   * Add a new dispatchable object
   *
   * @retval true Added successfully
   */
  bool add(IDispatchable*);

  /**
   * Empties the entire map
   */
  void clear(void);

  /**
   * Determine the number of objects in the map
   *
   * @return Number of objects in the map
   */
  size_t size(void) const
  {
    return objectmap.size();
  }
};

#endif // __DISPATCHER_H__

// vim:cin:ai:sts=2 sw=2 ft=cpp
