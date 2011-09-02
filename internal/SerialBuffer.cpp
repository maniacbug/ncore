#include <string>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>

#include <Logger.h>
#include <Dispatcher.h>
#include <SerialBuffer.h>

using namespace std;

static SerialBuffer* global_object = NULL;

void SerialBuffer::put(const string& str)
{
  log.add(str); 
}
void SerialBuffer::put(char c)
{
  if ( c == '\n' )
  {
    log.add(outstream.str());
    outstream.str(string());
  }
  else
    outstream << c;
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
