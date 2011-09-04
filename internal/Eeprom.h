#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <vector>
#include <inttypes.h>

class Logger;

class Eeprom
{
private:
  std::vector<uint8_t> values;
  Logger& log;
public:
  Eeprom(Logger&);
  size_t size(void);
  uint8_t readByte(int addr) const;
  void writeByte(int addr, uint8_t value);
};

#endif // __EEPROM_H__
