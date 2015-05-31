
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <sstream>

#include <IDispatchable.h>

class Logger;
class Parser;

/**
 * Mimics the Arduino Hardware Serial.  Provides an interface to mock
 * the inputs to Arduino serial.  Logs the outputs.
 */

class SerialBuffer: public IDispatchable
{
  Logger& log;
  std::ostringstream outstream;
  std::istringstream instream;
  bool outstream_has_data;

protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );

  bool command_send(const std::vector<std::string>&);
  bool command_sendhex(const std::vector<std::string>&);

public:
  SerialBuffer(Logger& _log): log(_log), outstream_has_data(false) {}
  size_t put(const std::string &);
  size_t put(char);
  bool available(void) const;
  char get(void);
  void flush(void);
  char peek(void) const;

  void setInput(const std::string&);
};

#endif // __SERIAL_BUFFER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp

