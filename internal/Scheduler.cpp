// STL includes
// C includes
// Library includes
// Project includes
#include <Scheduler.h>

void Scheduler::runonce(void)
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

void Scheduler::add(unsigned long trigger_at ,const std::string& commands )
{
  // TODO: object_q_mutex
  object_q.push(SchedulableObject(trigger_at,commands));
  //post();
}

size_t Scheduler::size(void) const
{
  return object_q.size();
}

void Scheduler::clear(void)
{
  clock = Clock();
  while ( ! object_q.empty() )
    object_q.pop();
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
