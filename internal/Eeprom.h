#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <inttypes.h>

class Eeprom
{
public:
  size_t size(void) { return 1; }
};

#endif // __EEPROM_H__
