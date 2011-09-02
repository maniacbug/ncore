#include <string>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>

#include <inttypes.h>

#include <Logger.h>
#include <Dispatcher.h>
#include <SerialBuffer.h>
#include <Clock.h>

using namespace std;

static SerialBuffer* global_object = NULL;
static Clock internalClock;
static unsigned long last_logged_at = 0;

//
// Public interface
//

void SerialBuffer::flush(void)
{
  log.add(outstream.str());
  outstream.str(string());
  outstream_has_data = false;
}

void SerialBuffer::put(const string& str)
{
  log.add(str); 
}
void SerialBuffer::put(char c)
{
  if ( c == '\n' ) 
  {
    flush();
  }
  else if ( c == '\r' )
  {
    // ignore \r's
  }
  else if ( c < ' ' || c > 'z' )
  {
    outstream << "[0x" << setbase(16) << (unsigned int)(unsigned char)c << "]" ;
    if ( !outstream_has_data )
      last_logged_at = internalClock.millis();
    outstream_has_data = true;
  }
  else
  {
    outstream << c;
    if ( !outstream_has_data )
      last_logged_at = internalClock.millis();
    outstream_has_data = true;
  }

  if ( outstream_has_data && internalClock.millis() - last_logged_at > 1000 )
    flush();
}

bool SerialBuffer::available(void) const
{
  return instream.rdbuf()->in_avail(); 
}
  
void SerialBuffer::setInput(const std::string& instr)
{
  instream.str(instr);
}

char SerialBuffer::get(void)
{
  char result;
  instream.readsome(&result,1);
  return result;
}

void SerialBuffer::addCommandsTo(Dispatcher& _commands)
{
  global_object = this;
  _commands.add("send",SerialBuffer::static_command_send);
}

bool SerialBuffer::static_command_send(const vector<string>& _commands)
{
  if ( ! global_object )
    throw new runtime_error("No serial buffer registered to receive commands");

  return global_object->command_send(_commands);
}

bool SerialBuffer::command_send(const vector<string>& _commands)
{
  if ( _commands.size() < 2 )
    throw new runtime_error("Usage: send <message>");

  ostringstream composite;
  copy(_commands.begin() + 1, _commands.end(),ostream_iterator<string>(composite," "));
  string composite_str = composite.str();
  // Trim off the trailing " " from the last ostream iterator
  setInput(composite_str.substr(0,composite_str.size()-1));
  
  return true;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
