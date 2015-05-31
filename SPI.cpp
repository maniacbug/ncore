/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <SpiQueue.h>
#include <Arduino.h>
#include "pins_arduino.h"
#include "SPI.h"

SPIClass SPI;

extern SpiQueue theSpiQueue;

void SPIClass::begin() {
}

void SPIClass::end() {
}

void SPIClass::setBitOrder(uint8_t)
{
}

void SPIClass::setDataMode(uint8_t)
{
}

void SPIClass::setClockDivider(uint8_t)
{
}

byte SPIClass::transfer(byte _data) {
  return theSpiQueue.transfer(_data);
}
