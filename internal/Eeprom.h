#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <vector>
#include <inttypes.h>
#include <IDispatchable.h>

class Logger;

/**
 * Mimics the Arduino EEPROM
 */

class Eeprom: public IDispatchable
{
private:
  std::vector<uint8_t> values;
  Logger& log;
protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );
  bool command_eeprom(const std::vector<std::string>&);
public:
  Eeprom(Logger&);
  size_t size(void);
  uint8_t readByte(int addr) const;
  void writeByte(int addr, uint8_t value);
};

#endif // __EEPROM_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
