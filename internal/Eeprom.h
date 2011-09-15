
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <vector>
#include <inttypes.h>
#include <IDispatchable.h>

class Logger;

/**
 * Mimics the Arduino EEPROM
 */

class Eeprom: public IDispatchable
{
private:
  std::vector<uint8_t> values; /**< Eeprom memory mock */
  Logger& log; /**< Where to log changes */
protected:
  /**
   * Discover which commands this objects will dispatch
   *
   * @return Space-separated list of commands we will dispatch
   */
  std::string& getCommands(void) const;

  /**
   * Run a command the user has typed
   *
   * @param commands Parsed command and arguments
   *
   * @retval true Found the command and it was successful
   */
  bool runCommand( const Parser& commands );

  /**
   * Handle the 'eeprom' command
   *
   * @param commands Parsed command and arguments
   *
   * @retval true The command was successful
   */
  bool command_eeprom(const std::vector<std::string>&);
public:
  Eeprom(Logger&);
  size_t size(void);
  uint8_t readByte(int addr) const;
  void writeByte(int addr, uint8_t value);
  void clear(void);
};

#endif // __EEPROM_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
