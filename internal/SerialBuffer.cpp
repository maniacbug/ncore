// STL includes
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
// C includes
#include <inttypes.h>
#include <cstring>
// Library includes
// Project includes
#include <Logger.h>
#include <Dispatcher.h>
#include <Clock.h>
#include <Parser.h>
#include <SerialBuffer.h>

/****************************************************************************/

using namespace std;

/****************************************************************************/

static Clock internalClock;
static unsigned long last_logged_at = 0;

/****************************************************************************/

//
// Public interface
//

/****************************************************************************/

void SerialBuffer::flush(void)
{
  log.sketch("SERL",outstream.str());
  outstream.str(string());
  outstream_has_data = false;
}

/****************************************************************************/

size_t SerialBuffer::put(const string& str)
{
  // WTF is this for??!!
  log.sketch("SERL", str);
  return strlen(str.c_str());
}

/****************************************************************************/

size_t SerialBuffer::put(char c)
{
  size_t n = 0;
  if ( outstream_has_data && internalClock.millis() - last_logged_at > 1000 )
  {
    outstream << "[nocr]";
    flush();
    n+=6;
  }

  if ( c == '\n' )
  {
    flush();
    n+=1;
  }
  else if ( c == '\r' )
  {
    // ignore \r's
  }
  else if ( c < ' ' || c > 'z' )
  {
    outstream << "[0x" << setbase(16) << (unsigned int)(unsigned char)c << "]" ;
    n+= 4 + strlen((const char*)&c);
    if ( !outstream_has_data )
      last_logged_at = internalClock.millis();
    outstream_has_data = true;
  }
  else {
    n+= strlen((const char*)&c);
    outstream << c;
    if ( !outstream_has_data )
      last_logged_at = internalClock.millis();
    outstream_has_data = true;
  }
  return n;

}

/****************************************************************************/

bool SerialBuffer::available(void) const
{
  return instream.rdbuf()->in_avail();
}

/****************************************************************************/

void SerialBuffer::setInput(const std::string& instr)
{
  instream.str(instr);
}

/****************************************************************************/

char SerialBuffer::get(void)
{
  char result;
  instream.readsome(&result,1);
  return result;
}

/****************************************************************************/

char SerialBuffer::peek(void) const
{
  SerialBuffer* nonconst_this = const_cast<SerialBuffer*>(this);
  return nonconst_this->instream.peek();
}

/****************************************************************************/

string& SerialBuffer::getCommands(void) const
{
  static std::string commands = "send send-hex";
  return commands;
}

/****************************************************************************/

bool SerialBuffer::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "send" )
  {
    result = command_send(parser);
  }
  else if ( command == "send-hex" )
  {
    result = command_sendhex(parser);
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);
    if ( command == "send" )
    {
      cout << "send <data> [cr] -- send arbitrary data via Serial, optional 'cr' at end." << endl;
    }
    else if ( command == "send-hex" )
    {
      cout << "send-hex <xx> .. <xx> -- send hex digits, separate them by spaces." << endl;
    }
  }

  return result;
}

/****************************************************************************/

bool SerialBuffer::command_sendhex(const vector<string>& _commands)
{
  if ( _commands.size() < 2 )
    throw new runtime_error("Usage: send-hex <message>");

  string composite_str;
  vector<string>::const_iterator it = _commands.begin() + 1;
  while (it != _commands.end())
  {
    int i;
    istringstream convert(*it++);
    convert >> hex >> i;
    composite_str.push_back(i);
  }

  // Also log the command
  log.internal("SERL","send-hex %i chars",composite_str.size());

  // set as input
  setInput(composite_str);

  return true;
}

/****************************************************************************/

bool SerialBuffer::command_send(const vector<string>& _commands)
{
  if ( _commands.size() < 2 )
    throw new runtime_error("Usage: send <message>");

  // If ends in 'cr', we want to send an \n at the end.
  int hascr = 0;
  if ( _commands.at(_commands.size()-1) == "cr" )
    hascr = 1;

  ostringstream composite;
  copy(_commands.begin() + 1, _commands.end() - hascr,ostream_iterator<string>(composite," "));
  string composite_str = composite.str();

  // Trim off the trailing " " from the last ostream iterator
  composite_str.resize(composite_str.size()-1);

  // Also log the command
  log.internal("SERL","send %s %s",composite_str.c_str(),hascr?"(with CR)":"(no CR)");

  // Add the trailing cr if needed
  if ( hascr )
    composite_str.push_back('\n');

  // set as input
  setInput(composite_str);

  return true;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
