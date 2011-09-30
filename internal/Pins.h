
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __PINS_H__
#define __PINS_H__

#include <vector>
#include <string>
#include <map>

#include <IDispatchable.h>

class Logger;

#ifndef OUTPUT
const int OUTPUT = 1;
#endif
#ifndef INPUT
const int INPUT = 0;
#endif

/**
 * Mimics the Arduino GPIO pins
 */

class Pins: public IDispatchable
{
private:
  static const int num_pins = 20;
  static const int num_channels = 8;
  static const int num_interrupts = 2;
  std::vector<int> digital_states;
  std::vector<int> pin_modes;
  std::vector<int> analog_states;
  std::vector<void (*)(void)> isr_table;

  // Not sure this is the best way to deal with it, but
  // it's the fastest!!
  std::map<int,std::string> symbol_map;
  std::map<std::string,int> symbol_reverse_map; 

  Logger& log;

protected:
  // What string should we use to log this pin?
  std::string pin_log_name(int) const;
  std::string& getCommands() const
  {
    static std::string commands = "pins pin irq (default)";
    return commands;
  }
  bool runCommand( const Parser& );

  bool command_pins(const std::vector<std::string>&) const;

  bool command_pin(const std::vector<std::string>&);
  bool command_pin_digital(std::vector<std::string>::const_iterator current,std::vector<std::string>::const_iterator end );
  bool command_pin_analog(std::vector<std::string>::const_iterator current,std::vector<std::string>::const_iterator end );

  bool command_irq(const std::vector<std::string>&) const;
public:
  Pins(Logger&);
  void clear(void);
  int digitalRead(int pin) const;
  void hwSetDigital(int pin,int level);
  int analogRead(int pin) const;
  void hwSetAnalog(int pin,int level);
  void attachInterrupt(int irq, void (*isr)(void));
  void detachInterrupt(int irq);
  void hwTriggerInterrupt(int irq) const;
  void digitalWrite(int pin,int level);
  int hwGetDigital(int pin) const;
  void pinMode(int pin, int dir);
  void pinSymbol(int pin, const std::string& symbol);
};

#endif // __PINS_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
