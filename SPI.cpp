/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <WProgram.h>
#include "pins_arduino.h"
#include "SPI.h"

SPIClass SPI;

void SPIClass::begin() {
}

void SPIClass::end() {
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
}

void SPIClass::setDataMode(uint8_t mode)
{
}

void SPIClass::setClockDivider(uint8_t rate)
{
}

byte SPIClass::transfer(byte _data) {
  printf("NCORE: %06lu ",millis());
  printf("SPI transfer %02x\n",_data);

  return 0;
}
