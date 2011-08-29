#include <iostream>
#include <Pins.h>
#include <Commands.h>

using namespace std;

static const Pins* global_pins = NULL;

Pins::Pins(void)
{
  clear();
}
void Pins::clear(void)
{
  digital_states.clear();
  analog_states.clear();

  digital_states.resize(num_pins);
  analog_states.resize(num_channels);
}
int Pins::digitalRead(int pin) const
{
  return digital_states[pin];
}
void Pins::hwSetDigital(int pin,int level)
{
  digital_states[pin] = level;
}
int Pins::analogRead(int pin) const
{
  return analog_states[pin];
}
void Pins::hwSetAnalog(int pin,int level)
{
  analog_states[pin] = level;
}

bool Pins::static_command_pins(const vector<string>& _commands)
{
  if ( ! global_pins )
    return false;

  return global_pins->command_pins(_commands);
}

bool Pins::command_pins(const vector<string>& _commands) const
{
  int counter = 0;
  cout << "DIGITAL PINS: ";
  vector<int>::const_iterator digital = digital_states.begin();
  while ( digital != digital_states.end() )
  {
    cout << counter++ << ":" << *digital << " ";
    ++digital;
  }
  cout << endl;
  counter = 0;
  cout << "ANALOG PINS: ";
  vector<int>::const_iterator analog = analog_states.begin();
  while ( analog != analog_states.end() )
  {
    cout << counter++ << ":" << *analog << " ";
    ++analog;
  }
  cout << endl;

  return true;
}

void Pins::addCommandsTo(Commands& _commands) const
{
  global_pins = this;
  _commands.add("pins",Pins::static_command_pins);
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
