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
  void runonce(void)
  {
    //if ( object_q.empty() )
    //  wait();
    if ( ! object_q.empty() )
    {
      unsigned long top_trigger_at = object_q.top().trigger_at;
      unsigned long now = clock.millis();
      if ( now >= top_trigger_at )
      {
	// TODO: object_q_mutex
	SchedulableObject o = object_q.top();
	object_q.pop();

	dispatch.execute(o.commands);
      }
      //else
      //{
	//wait(top_trigger_at - time);
      //}
    }
  }
public:
  Scheduler(Dispatcher& _d): dispatch(_d) {}
  void add(unsigned long trigger_at ,const std::string& commands )
  {
    // TODO: object_q_mutex
    object_q.push(SchedulableObject(trigger_at,commands));
    //post();
  }
  size_t size(void) const
  {
    return object_q.size();
  }
  void clear(void)
  {
    clock = Clock();
    while ( ! object_q.empty() )
      object_q.pop();
  }
};

#endif // __SCHEDULER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
