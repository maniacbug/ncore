// STL includes
#include <iostream>
#include <stdexcept>
#include <sstream>
// C includes
// Library includes
// Project includes
#include <Dispatcher.h>
#include <Parser.h>
#include <Logger.h>
#include <Pins.h>

using namespace std;

/****************************************************************************/

const int LOW = 0;
const int HIGH = 1;

/****************************************************************************/

string Pins::pin_log_name(int pin) const
{
  ostringstream os;

  if ( symbol_map.count(pin) )
    os << symbol_map.at(pin);
  else
    os << dec << pin;

  return os.str();
}

/****************************************************************************/

Pins::Pins(Logger& _log): log(_log)
{
  clear();
}

/****************************************************************************/

void Pins::clear(void)
{
  digital_states.clear();
  pin_modes.clear();
  analog_states.clear();
  isr_table.clear();
  symbol_map.clear();
  symbol_reverse_map.clear();

  digital_states.resize(num_pins);
  pin_modes.resize(num_pins);
  analog_states.resize(num_channels);
  isr_table.resize(num_interrupts);
}

/****************************************************************************/

int Pins::digitalRead(int pin) const
{
  int result = digital_states.at(pin);
  return result;
}

/****************************************************************************/

void Pins::hwSetDigital(int pin,int level)
{
  digital_states.at(pin) = level;
  log.internal("PINS","%s %s",pin_log_name(pin).c_str(),level?"HIGH":"LOW");
}

/****************************************************************************/

int Pins::analogRead(int pin) const
{
  return analog_states.at(pin);
}

/****************************************************************************/

void Pins::hwSetAnalog(int pin,int level)
{
  analog_states.at(pin) = level;
  log.internal("PINS", "A%i %i",pin,level);
}

/****************************************************************************/

void Pins::digitalWrite(int pin,int level)
{
  if ( pin_modes.at(pin) == OUTPUT )
  {
    digital_states.at(pin) = level;
    log.sketch("PINS","%s %s",pin_log_name(pin).c_str(),level?"HIGH":"LOW");
  }
}

/****************************************************************************/

int Pins::hwGetDigital(int pin) const
{
  return digital_states.at(pin);
}

/****************************************************************************/

void Pins::pinMode(int pin, int dir)
{
  pin_modes.at(pin) = dir;
  log.sketch("PINS","%s %s",pin_log_name(pin).c_str(),dir?"OUTPUT":"INPUT");
}

/****************************************************************************/

void Pins::pinSymbol(int pin, const std::string& symbol)
{
  symbol_map[pin] = symbol;
  symbol_reverse_map[symbol] = pin;
}

/****************************************************************************/

void Pins::attachInterrupt(int irq, void (*isr)(void))
{
  isr_table.at(irq) = isr;
  log.sketch("IRQ","%i attached",irq);
}

/****************************************************************************/

void Pins::detachInterrupt(int irq)
{
  attachInterrupt(irq,NULL);
  log.sketch("IRQ","%i detached",irq);
}

/****************************************************************************/

void Pins::hwTriggerInterrupt(int irq) const
{
  if ( ! isr_table.at(irq) )
    throw new runtime_error("No handler assigned for this interrupt");

  log.internal("IRQ","%i triggered",irq);
  isr_table[irq]();
}

/****************************************************************************/

bool Pins::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "pins" )
  {
    result = command_pins(parser);
  }
  else if ( command == "pin" )
  {
    result = command_pin(parser);
  }
  else if ( command == "irq" )
  {
    result = command_irq(parser);
  }
  else if ( command == "help" )
  {
    const string& helpcommand = parser.at(1);

    if ( helpcommand == "pins" )
    {
      cout << "pins -- list current state of all pins" << endl;
    }
    else if ( helpcommand == "pin" )
    {
      cout << "pin <#> HIGH|LOW -- set digital pin high or low" << endl;
      cout << "pin A<#> <value> -- set analog pin to value" << endl;
    }
    else if ( helpcommand == "irq" )
    {
      cout << "irq <#> -- trigger irq <#>" << endl;
    }
    result = true;
  }

  return result;
}

/****************************************************************************/

bool Pins::command_pin_digital(vector<string>::const_iterator current,vector<string>::const_iterator end)
{
  int pin;
  char c = (*current)[0];
  if ( c >= '0' && c <= '9' )
  {
    istringstream ss(*current);
    ss >> pin;
  }
  else
  {
    // This is a pin symbol
    if ( symbol_reverse_map.count(*current) )
      pin = symbol_reverse_map.at(*current);
    else
      throw new runtime_error("Unknown pin value");
  }
  current++;

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

/****************************************************************************/

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

/****************************************************************************/

bool Pins::command_pin(const vector<string>& _commands)
{
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

/****************************************************************************/

bool Pins::command_pins(const vector<string>&) const
{
  int counter = 0;
  cout << "DIGITAL PINS: ";
  vector<int>::const_iterator digital = digital_states.begin();
  while ( digital != digital_states.end() )
  {
    cout << dec << counter++ << ":" << *digital << " ";
    ++digital;
  }
  cout << endl;
  counter = 0;
  cout << "ANALOG PINS: ";
  vector<int>::const_iterator analog = analog_states.begin();
  while ( analog != analog_states.end() )
  {
    cout << dec << counter++ << ":" << *analog << " ";
    ++analog;
  }
  cout << endl;

  return true;
}

/****************************************************************************/

bool Pins::command_irq(const vector<string>& _commands) const
{
  vector<string>::const_iterator current = _commands.begin();

  // Skip over 'irq' command
  ++current;

  // Get the irq number
  if ( current == _commands.end() )
    throw new runtime_error("Expecting irq number");

  char c = (*current)[0];
  if ( c < '0' || c > '9' )
    throw new runtime_error("Unknown irq number");

  istringstream ss(*current++);
  int irq;
  ss >> irq;

  if ( irq < 0 || irq >= num_interrupts )
    throw new runtime_error("Interrupt number out of range");

  // Make sure we're at the end of input
  if ( current != _commands.end() )
    throw new runtime_error("Unexpected tokens at end of input");

  hwTriggerInterrupt( irq );

  return true;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
