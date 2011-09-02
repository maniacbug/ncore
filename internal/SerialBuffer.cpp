#include <string>

#include <Logger.h>
#include <SerialBuffer.h>

using namespace std;

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
// vim:cin:ai:sts=2 sw=2 ft=cpp
