
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <sys/time.h>

/**
 * Mimics Arduino clock-related routines.  Mainly provides a millis() counter.
 */

class Clock
{
private:
  struct timeval program_start; /**< Absolute time when object was constructed. */
public:
  /**
   * Starts a blank clock.  Assumes the program starts upon construction
   */
  Clock(void);

  /**
   * Wait for some time
   *
   * @param ms Time to wait in milliseconds
   */
  void delay(unsigned long ms) const;
  
  /**
   * Wait for some time
   *
   * @param us Time to wait in microseconds
   */
  void delayMicroseconds(unsigned int us) const;

  /**
   * Find time elapsed since program start
   *
   * @return Number of milliseconds elapsed since this object was
   * constructed.
   */
  unsigned long millis(void) const;
};
#endif // __CLOCK_H__
// vim:cin:ai:sts=2 sw=2
