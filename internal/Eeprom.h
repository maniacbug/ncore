#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <vector>
#include <inttypes.h>

class Eeprom
{
private:
  std::vector<uint8_t> values;
public:
  Eeprom(void);
  size_t size(void);
  uint8_t readByte(int addr) const;
  void writeByte(int addr, uint8_t value);
};

#endif // __EEPROM_H__
