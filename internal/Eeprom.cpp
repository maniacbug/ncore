#include <Eeprom.h>
#include <Logger.h>

// eeprom size must always be a power of 2
static const int eeprom_size = 1 << 9;

Eeprom::Eeprom(Logger& _log): log(_log)
{
  values.resize(eeprom_size);
}
size_t Eeprom::size(void)
{
  return values.size();
}
uint8_t Eeprom::readByte(int addr) const
{
  uint8_t result = values.at(addr);
  log.sketch("EEPR","read %i = %u",addr,result);
  return result;
}
void Eeprom::writeByte(int addr, uint8_t value)
{
  log.sketch("EEPR","write %i = %u",addr,value);
  values.at(addr) = value;
}
