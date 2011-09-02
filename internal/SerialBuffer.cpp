#include <string>

#include <Logger.h>
#include <SerialBuffer.h>

using namespace std;

void SerialBuffer::put(const string& str)
{
  log.add(str); 
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
