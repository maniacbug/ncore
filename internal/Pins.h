#ifndef __PINS_H__
#define __PINS_H__

#include <vector>

class Pins
{
protected:
  static const int num_pins = 20;
  std::vector<int> digital_pin_states;
public:
  Pins(void) { digital_pin_states.reserve(num_pins); }
  int digitalRead(int pin) { return digital_pin_states[pin]; }
  void hwSetDigital(int pin,int level) { digital_pin_states[pin] = level; }
};

#endif // __PINS_H__
