#include <vector>
#include <inttypes.h>
#include <EepromInternal.h>

extern Eeprom theEeprom;

extern "C"
{

void __eewr_byte_native (uint8_t *__p, uint8_t __value)
{
  int addr = reinterpret_cast<long>(__p);
  theEeprom.writeByte(addr,__value);
}
uint8_t __eerd_byte_native (const uint8_t *__p)
{
  int addr = reinterpret_cast<long>(__p);
  uint8_t result = theEeprom.readByte(addr);
  return result;
}

}
// vim:cin:ai:sts=2 sw=2 ft=cpp
