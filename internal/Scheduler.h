#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

// STL includes
#include <queue>
#include <string>

// C includes
// Library includes
// Project includes
#include <Clock.h>

struct SchedulableObject
{
  unsigned long trigger_at;
  std::string commands;
  SchedulableObject( unsigned long _t, const std::string& _p ): trigger_at(_t), commands(_p) {}
  // lesser value means later in time
  bool operator<(const SchedulableObject& _rhs) const { return trigger_at > _rhs.trigger_at; }
};

//class Dispatcher;
#include <Dispatcher.h>

class Scheduler
{
private:
  std::priority_queue<SchedulableObject> object_q;
  Dispatcher& dispatch;
  Clock clock; // TODO: Need to pass this in!!
public:
//protected:
  void runonce(void);
public:
  Scheduler(Dispatcher& _d): dispatch(_d) {}
  void add(unsigned long trigger_at, const std::string& commands );
  size_t size(void) const;
  void clear(void);
};

#endif // __SCHEDULER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
