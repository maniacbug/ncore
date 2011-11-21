/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __WIREQUEUE_H__
#define __WIREQUEUE_H__

// STL headers
// C headers
// Framework headers
#include <Logger.h>
#include <QueueTS.h>
// Library headers
// Project headers

/**
 * Example for how classes should be declared
 */

class WireQueue
{
private:
  QueueTS<uint8_t> qts;
  Logger& logger;
protected:
public:
  WireQueue(Logger& _logger): logger(_logger) {}
  void send(uint8_t);
};

#endif // __WIREQUEUE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
