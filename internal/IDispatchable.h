#ifndef __IDISPATCHABLE_H__
#define __IDISPATCHABLE_H__

#include <string>

class Parser;

class IDispatchable
{
public:
  virtual std::string& getCommands(void) const = 0;
  virtual bool runCommand( const Parser& ) = 0;
};

#endif // __IDISPATCHABLE_H__
