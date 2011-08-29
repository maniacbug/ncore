#ifndef __PINS_H__
#define __PINS_H__

#include <vector>

class Commands;

class Pins
{
protected:
  static const int num_pins = 20;
  static const int num_channels = 8;
  std::vector<int> digital_states;
  std::vector<int> analog_states;

  static bool static_command_pins(const std::vector<std::string>&);
  bool command_pins(const std::vector<std::string>&) const;
public:
  Pins(void);
  void clear(void);
  int digitalRead(int pin) const;
  void hwSetDigital(int pin,int level);
  int analogRead(int pin) const;
  void hwSetAnalog(int pin,int level);

  void addCommandsTo(Commands&) const;
};

#endif // __PINS_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
