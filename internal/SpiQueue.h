
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SPI_QUEUE_H__
#define __SPI_QUEUE_H__

#include <queue>
#include <string>

#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>

#include <QueueTS.h>
#include <IDispatchable.h>

class Logger;

/**
 * Mimics Arduino SPI interface.  Provides a means to queue up SPI responses
 * and logs SPI transfers.
 */

class SpiQueue: public IDispatchable
{
private:
  QueueTS<uint8_t> qts;
  Logger& logger;
  bool has_default;
  uint8_t default_value;
protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );
  bool command_spi(const std::vector<std::string>&);
public:
  SpiQueue(Logger&);
  void hwEnqueue(uint8_t);
  uint8_t transfer(uint8_t);
  void clear(void);
};

#endif // __SPI_QUEUE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
