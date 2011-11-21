/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

// STL headers
// C headers
// Framework headers
#include <WireQueue.h>
// Library headers
// Project headers
// This component's header
#include <Wire.h>

TwoWire Wire;

extern WireQueue theWireQueue;

/****************************************************************************/
TwoWire::TwoWire()
{
}
void TwoWire::begin()
{
}
void TwoWire::begin(uint8_t)
{
}
void TwoWire::begin(int)
{
}
void TwoWire::beginTransmission(uint8_t)
{
}
void TwoWire::beginTransmission(int)
{
}
uint8_t TwoWire::endTransmission(void)
{
  return 0;
}
uint8_t TwoWire::requestFrom(uint8_t, uint8_t)
{
  return 0;
}
uint8_t TwoWire::requestFrom(int, int)
{
  return 0;
}
void TwoWire::send(uint8_t val)
{
  theWireQueue.send(val);
}
void TwoWire::send(uint8_t*, uint8_t)
{
}
void TwoWire::send(int)
{
}
void TwoWire::send(char*)
{
}
uint8_t TwoWire::available(void)
{
  return 0;
}
uint8_t TwoWire::receive(void)
{
  return theWireQueue.receive();
}
void TwoWire::onReceive( void (*)(int) )
{
}
void TwoWire::onRequest( void (*)(void) )
{
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
