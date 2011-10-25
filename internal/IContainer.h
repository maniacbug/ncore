
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __ICONTAINER_H__
#define __ICONTAINER_H__

/**
 * Interface representing a generic 'container'
 */
class IContainer
{
public:
  /**
   * Number of units currently in the container
   */
  virtual size_t size(void) const = 0;
};

#endif // __ICONTAINER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
