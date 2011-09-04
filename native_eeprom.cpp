#include <vector>
#include <inttypes.h>
#include <Logger.h>

extern Logger theLogger;

// eeprom size must always be an even power of 2
const int eep_size = 1 << 10;

struct eep_t: public std::vector<uint8_t>
{
  eep_t(void)
  {
    resize(eep_size);
  }
};

static eep_t eep;

extern "C"
{

void __eewr_byte_native (uint8_t *__p, uint8_t __value)
{
  int addr = reinterpret_cast<int>(__p);
  theLogger.sketch("EEPR","write %i = %u",addr,__value);
  eep.at(addr) = __value;
}
uint8_t __eerd_byte_native (const uint8_t *__p)
{
  int addr = reinterpret_cast<int>(__p);
  uint8_t result = eep.at(addr);
  theLogger.sketch("EEPR","read %i = %u",addr,result);
  return result;
}

}
// vim:cin:ai:sts=2 sw=2 ft=cpp
