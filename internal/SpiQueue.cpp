#include <iostream>
#include <sstream>
#include <stdexcept>

#include <Parser.h>
#include <Logger.h>
#include <SpiQueue.h>

using namespace std;
  
SpiQueue::SpiQueue(Logger& _logger): logger(_logger)
{
}

void SpiQueue::hwEnqueue(uint8_t _byte)
{
  qts.push(_byte);
}
  
uint8_t SpiQueue::transfer(uint8_t _in)
{
  if ( ! qts.available() )
    logger.sketch("SPI","in %02x Waiting for output values",_in);
  uint8_t out = qts.pop();
  logger.sketch("SPI","in %02x out %02x",_in,out);
  return out;
}

string& SpiQueue::getCommands(void) const 
{ 
  static std::string commands = "spi"; 
  return commands; 
}

bool SpiQueue::runCommand( const Parser& parser ) 
{ 
  bool result = false;
  
  const string& command = parser.at(0);

  if ( command == "spi" )
  {
    result = command_spi(parser);
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);
    if ( command == "spi" )
    {
      cout << "spi <xx> .. <xx> -- add data to spi output queue." << endl;
    }
  }

  return result; 
}

bool SpiQueue::command_spi(const vector<string>& _commands)
{
  if ( _commands.size() < 2 )
    throw new runtime_error("Usage: spi <xx> .. <xx>");

  vector<uint8_t> composite;
  vector<string>::const_iterator it = _commands.begin() + 1;
  while (it != _commands.end())
  {
    int i;
    istringstream convert(*it++);
    convert >> hex >> i;
    composite.push_back(i);
  }

  // Also log the command
  logger.internal("SPI","queued %i bytes for output",composite.size());
  
  // set as input
  vector<uint8_t>::const_iterator curbyte = composite.begin();
  while (curbyte != composite.end())
    qts.push(*curbyte++);
  
  return true;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
