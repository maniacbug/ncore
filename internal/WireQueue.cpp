/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

// STL headers
// C headers
// Framework headers
// Library headers
// Project headers
// This component's header
#include <WireQueue.h>

/****************************************************************************/

void WireQueue::send(uint8_t out)
{
  logger.sketch("WIRE","send %02x",out);
}

/****************************************************************************/

uint8_t WireQueue::receive(void)
{
  uint8_t result = 0;
  if ( ! qts.available() )
    logger.sketch("WIRE","Waiting for output values");
  result = qts.pop();
  logger.sketch("WIRE","received %02x",result);

  return result;
}

/****************************************************************************/

void WireQueue::hwEnqueue(uint8_t _byte)
{
  qts.push(_byte);
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp

