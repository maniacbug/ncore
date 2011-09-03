#ifndef __PINS_H__
#define __PINS_H__

#include <vector>
#include <IDispatchable.h>

class Dispatcher;

#ifndef OUTPUT
const int OUTPUT = 1;
#endif
#ifndef INPUT 
const int INPUT = 0; 
#endif

class Pins: public IDispatchable
{
protected:
  static const int num_pins = 20;
  static const int num_channels = 8;
  static const int num_interrupts = 2;
  std::vector<int> digital_states;
  std::vector<int> pin_modes;
  std::vector<int> analog_states;
  std::vector<void (*)(void)> isr_table;

  static void reset(void); /**< Reset state of statics. Only for testing! */

  static bool static_command_pins(const std::vector<std::string>&);
  bool command_pins(const std::vector<std::string>&) const;

  static bool static_command_pin(const std::vector<std::string>&);
  bool command_pin(const std::vector<std::string>&);
  bool command_pin_digital(std::vector<std::string>::const_iterator current,std::vector<std::string>::const_iterator end );
  bool command_pin_analog(std::vector<std::string>::const_iterator current,std::vector<std::string>::const_iterator end );
  
  static bool static_command_irq(const std::vector<std::string>&);
  bool command_irq(const std::vector<std::string>&) const;
public:
  Pins(void);
  void clear(void);
  int digitalRead(int pin) const;
  void hwSetDigital(int pin,int level);
  int analogRead(int pin) const;
  void hwSetAnalog(int pin,int level);
  void attachInterrupt(int irq, void (*isr)(void));
  void detachInterrupt(int irq);
  void hwTriggerInterrupt(int irq) const;
  void digitalWrite(int pin,int level);
  int hwGetDigital(int pin) const;
  void pinMode(int pin, int dir);
  
  void addCommandsTo(Dispatcher&);
  std::string& getCommands() const { static std::string commands = "pins pin irq"; return commands; }
  bool runCommand( const Parser& );
};

#endif // __PINS_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
