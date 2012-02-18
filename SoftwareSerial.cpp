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
#include <SoftwareSerial.h>

/****************************************************************************/
SoftwareSerial::SoftwareSerial(uint8_t /*receivePin*/, uint8_t /*transmitPin*/, bool /*inverse_logic*/ )
{
}
SoftwareSerial::~SoftwareSerial()
{
}
void SoftwareSerial::begin(long /*speed*/)
{
}
void SoftwareSerial::write(uint8_t /*byte*/)
{
}
void SoftwareSerial::flush()
{
}
int SoftwareSerial::read()
{
  return 32;
}
int SoftwareSerial::available()
{
  return 0;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
