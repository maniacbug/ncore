#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

// STL includes
#include <queue>
#include <string>

// C includes

// Library includes
#include <semaphore.h>

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

class Dispatcher;
class Logger;

class Scheduler
{
private:
  std::priority_queue<SchedulableObject> object_q;
  Dispatcher& dispatch;
  Logger& logger;
  Clock clock; // TODO: Need to pass this in!!
  sem_t sem;
public:
//protected:
  void runonce(void);
public:
  Scheduler(Dispatcher& _d, Logger& _l);
  virtual ~Scheduler();
  void add(unsigned long trigger_at, const std::string& commands );
  size_t size(void) const;
  void clear(void);
};

#endif // __SCHEDULER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
