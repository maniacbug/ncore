
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
public:
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
  /**
   * Constructor
   *
   * @param _log Where to log results
   */
  Eeprom(Logger& _log);
  
  /**
   * EEPROM Total Size
   *
   * @return Total size of EEPROM, whether used or not
   */
  size_t size(void);
  
  /**
   * Read from EEPROM
   *
   * @param addr Address within EEPROM to read from
   *
   * @result Value in EEPROM
   */
  uint8_t readByte(int addr) const;
  
  /**
   * Write to EEPROM
   *
   * @param addr Address within EEPROM
   * @param value Value to write
   */
  
  void writeByte(int addr, uint8_t value);
  
  /**
   * Reset all values to 0
   */
  void clear(void);

  /**
   * Beginning of eeprom space
   */
  std::vector<uint8_t>::const_iterator begin() const { return values.begin(); }

  /**
   * Ending of eeprom space
   */
  std::vector<uint8_t>::const_iterator end() const { return values.end(); }

};

/**
 * Serialize eeprom to output stream
 */
extern std::ostream& operator<<(std::ostream& os, const Eeprom& eep);

/**
 * Serialize eeprom from input stream
 */
extern std::istream& operator>>(std::istream& os, Eeprom& eep);

#endif // __EEPROM_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
