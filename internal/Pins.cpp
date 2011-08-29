#include <iostream>
#include <stdexcept>
#include <sstream>
#include <Pins.h>
#include <Dispatcher.h>

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
    throw new runtime_error("No pins registered to receive commands");

  return global_pins->command_pin(_commands);
}

bool Pins::static_command_pins(const vector<string>& _commands)
{
  if ( ! global_pins )
    throw new runtime_error("No pins registered to receive commands");

  return global_pins->command_pins(_commands);
}

bool Pins::command_pin_digital(vector<string>::const_iterator current,vector<string>::const_iterator end)
{
  char c = (*current)[0];
  if ( c < '0' || c > '9' )
    throw new runtime_error("Unknown pin value");

  istringstream ss(*current++);
  int pin;
  ss >> pin;

  if ( pin < 0 || pin >= num_pins )
    throw new runtime_error("Pin out of range");

  // Get the level, high or low
  if ( current == end )
    throw new runtime_error("Expecting pin level");

  int level;
  if ( *current == "high" )
    level = HIGH;
  else if ( *current == "low" )
    level = LOW;
  else
  {
    throw new runtime_error("Unknown level value");
  }

  ++current;

  // Make sure we're at the end of input
  if ( current != end )
    throw new runtime_error("Unexpected tokens at end of input");

  hwSetDigital( pin, level );

  return true;
}

bool Pins::command_pin_analog(vector<string>::const_iterator current,vector<string>::const_iterator end)
{
  string pin_str = (*current++).substr(1);
  istringstream ss(pin_str);
  int pin;
  ss >> pin;

  if ( pin < 0 || pin >= num_channels )
    throw new runtime_error("Analog channel out of range");

  if ( current == end )
    throw new runtime_error("Expecting pin level");

  char c = (*current)[0];
  if ( c < '0' || c > '9' )
    throw new runtime_error("Unknown level value");

  int level;
  istringstream ss_level(*current++);
  ss_level >> level;
  if ( level < 0 || level >= 1024 )
    throw new runtime_error("Level value out of range");

  // Make sure we're at the end of input
  if ( current != end )
    throw new runtime_error("Unexpected tokens at end of input");

  hwSetAnalog( pin, level );

  return true;
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

  // Is this an analog channel?
  string pin_str = *current;
  bool analog = ( pin_str[0] == 'a' );
  if ( analog )
    return command_pin_analog(current,_commands.end());
  else
    return command_pin_digital(current,_commands.end());
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

void Pins::addCommandsTo(Dispatcher& _commands)
{
  global_pins = this;
  _commands.add("pins",Pins::static_command_pins);
  _commands.add("pin",Pins::static_command_pin);
}
void Pins::reset(void)
{
  global_pins = NULL;
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
