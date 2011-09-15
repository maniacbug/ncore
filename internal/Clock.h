
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
 * Mimics Arduino clock-related routines
 */

class Clock
{
private:
  struct timeval program_start;
public:
  Clock(void);
  void delay(unsigned long ms) const;
  unsigned long millis(void) const;
  void delayMicroseconds(unsigned int us) const;
};
#endif // __CLOCK_H__
// vim:cin:ai:sts=2 sw=2
