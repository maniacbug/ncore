// STL includes
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
// C includes
// Library includes
// Project includes
#include <Logger.h>
#include <Parser.h>
#include <EepromInternal.h>

using namespace std;

/****************************************************************************/

// eeprom size must always be a power of 2
static const int eeprom_size = 1 << 9;

/****************************************************************************/

Eeprom::Eeprom(Logger& _log): log(_log)
{
  clear();
}

/****************************************************************************/

void Eeprom::clear(void)
{
  values.clear();
  values.resize(eeprom_size);
}

/****************************************************************************/

size_t Eeprom::size(void)
{
  return values.size();
}

/****************************************************************************/

uint8_t Eeprom::readByte(int addr) const
{
  uint8_t result = values.at(addr);
  log.sketch("EEPR","%i read %u",addr,result);
  return result;
}

/****************************************************************************/

void Eeprom::writeByte(int addr, uint8_t value)
{
  log.sketch("EEPR","%i write %u",addr,value);
  values.at(addr) = value;
}

/****************************************************************************/

std::string& Eeprom::getCommands(void) const
{
  static string commands = "eeprom ee";

  return commands;
}

/****************************************************************************/

bool Eeprom::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "eeprom" || command == "ee" )
  {
    result = command_eeprom(parser);
  }
  else if ( command == "help" )
  {
    const string& helpcommand = parser.at(1);

    if ( helpcommand == "eeprom" )
    {
      cout << "eeprom <addr> -- read one byte" << endl;
      cout << "eeprom <addr> len <len> -- read <len> bytes" << endl;
      cout << "eeprom <addr> write <xx> ... <xx> -- write hex bytes to eeprom" << endl;
    }
    result = true;
  }

  return result;
}

/****************************************************************************/

static int get_number(vector<string>::const_iterator current,vector<string>::const_iterator end )
{
  int result = 0;

  if ( current == end )
    throw new runtime_error("Expecting value");

  char c = (*current)[0];
  if ( c < '0' || c > '9' )
    throw new runtime_error("Expecing numeric value");

  istringstream ss(*current);
  ss >> result;

  return result;
}

/****************************************************************************/

static int get_hex_number(vector<string>::const_iterator current,vector<string>::const_iterator /*end*/ )
{
  int result = 0;

  /* Unreachable code
  if ( current == end )
    throw new runtime_error("Expecting value");
  */
  char c = (*current)[0];
  if ( ( c < '0' || c > '9' ) && ( c < 'a' || c > 'f' ) )
    throw new runtime_error("Expecing hexnumeric value");

  istringstream ss(*current);
  ss >> hex >> result;

  return result;
}

/****************************************************************************/

bool Eeprom::command_eeprom(const std::vector<std::string>& _commands)
{
  vector<string>::const_iterator current = _commands.begin() + 1;

  int addr = get_number(current++,_commands.end());

  if ( addr < 0 || addr >= eeprom_size )
    throw new runtime_error("Address out of range");

  if ( current == _commands.end() )
  {
    // read one byte
    cout << "EEPROM @ " << addr << " is " << hex << setfill('0') << setw(2) << (int)values.at(addr) << endl;
  }
  else
  {
    const string& operand = *current++;
    if ( operand == "len" )
    {
      // read len bytes
      int length = get_number(current++,_commands.end());
      cout << "EEPROM @ " << addr << " for " << dec << length << " are " << hex ;
      while( length-- )
        cout << setfill('0') << setw(2) << (int)values.at(addr++) << " ";
      cout << endl;
    }
    else if ( operand == "write" )
    {
      // write all the following bytes
      if ( current == _commands.end() )
        throw new runtime_error("Expecting at least one value to write");

      ostringstream message;
      message << addr << " write ";

      while ( current != _commands.end() )
      {
        int value = get_hex_number(current++,_commands.end());
        message << hex << setfill('0') << setw(2) << value << " ";
        values.at(addr++) = value;
      }
      cout << "EEPROM @ " << message.str() << endl;
      log.internal("EEPR",message.str());
    }
    else
      throw new runtime_error("Unknown sub-command");
  }

  return true;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
