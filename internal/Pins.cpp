#include <iostream>
#include <stdexcept>
#include <sstream>
#include <Pins.h>
#include <Commands.h>

using namespace std;

static Pins* global_pins = NULL;

const int LOW = 0;
const int HIGH = 1;

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

bool Pins::static_command_pin(const vector<string>& _commands)
{
  if ( ! global_pins )
    return false;

  return global_pins->command_pin(_commands);
}

bool Pins::static_command_pins(const vector<string>& _commands)
{
  if ( ! global_pins )
    return false;

  return global_pins->command_pins(_commands);
}

bool Pins::command_pin(const vector<string>& _commands)
{
  bool result = false;
  vector<string>::const_iterator current = _commands.begin();

  // Skip over 'pin' command
  ++current;

  // Get the pin number
  if ( current == _commands.end() )
    throw new runtime_error("Expecting pin number");
  
  istringstream ss(*current++);
  int pin;
  ss >> pin;

  if ( pin < 0 || pin >= num_pins )
    throw new runtime_error("Pin out of range");

  // Get the level, high or low
  if ( current == _commands.end() )
    throw new runtime_error("Expecting pin level");

  int level;
  if ( *current == "high" )
    level = HIGH;
  else if ( *current == "low" )
    level = LOW;
  else
    throw new runtime_error("Unknown level value");

  ++current;

  // Make sure we're at the end of input 
  if ( current != _commands.end() )
    throw new runtime_error("Unexpected tokens at end of input");

  hwSetDigital( pin, level );
  result = true;

  return result;
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

void Pins::addCommandsTo(Commands& _commands)
{
  global_pins = this;
  _commands.add("pins",Pins::static_command_pins);
  _commands.add("pin",Pins::static_command_pin);
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
