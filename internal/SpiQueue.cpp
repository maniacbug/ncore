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
  
uint8_t SpiQueue::transfer(uint8_t)
{
  uint8_t out = qts.pop();
  return out;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
