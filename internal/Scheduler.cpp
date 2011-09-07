// STL includes
#include <iostream>
#include <stdexcept>
// C includes
#include <time.h>
// Library includes
// Project includes
#include <Dispatcher.h>
#include <Scheduler.h>
#include <Logger.h>

using namespace std;

Scheduler::Scheduler(Dispatcher& _d, Logger& _l): dispatch(_d), logger(_l)
{
  sem_init(&sem,0,0);
}

Scheduler::~Scheduler()
{
  sem_destroy(&sem);
}

void Scheduler::runonce(void)
{
  if ( object_q.empty() )
  {
    int v;
    sem_getvalue(&sem,&v);
    cerr << "waiting sem=" << v <<  "..." << endl;
    sem_wait(&sem);
    cerr << "got sem..." << endl;
    logger.sketch("AT","Got sem");
  }
  if ( ! object_q.empty() )
  {
    unsigned long top_trigger_at = object_q.top().trigger_at;
    unsigned long now = clock.millis();
    cerr << "now " << now << " top " << top_trigger_at << endl;
    if ( now >= top_trigger_at )
    {
      // TODO: object_q_mutex
      SchedulableObject o = object_q.top();
      object_q.pop();

      cerr << "dispatch " << o.commands << "..." << endl;
      try
      {
	dispatch.execute(o.commands);
      }
      catch (runtime_error* e)
      {
	cerr << "runtime error " << e->what() << endl;
      }
    }
    else
    {
      timespec tm;
      unsigned long wait = top_trigger_at - now;
      clock_gettime(CLOCK_REALTIME,&tm);
      tm.tv_sec += wait / 1000L;
      tm.tv_nsec += (wait % 1000L) * 1000000L;
      if ( tm.tv_nsec >= 1000000000L )
      {
	tm.tv_sec += 1;
	tm.tv_nsec -= 1000000000L;
      }
      int v;
      sem_getvalue(&sem,&v);
      cerr << "waiting sem=" << v << " wait= " << wait << " timeout=" << tm.tv_sec << "." << tm.tv_nsec << " ..." << endl;
      logger.sketch("AT","Waiting %lu",wait);
      int result = sem_timedwait(&sem,&tm);
      if ( result )
	logger.sketch("AT","Sem timeout");
      else
	logger.sketch("AT","Got sem");
      cerr << "got sem " << result << " ..." << endl;
    }
  }
}

void Scheduler::add(unsigned long trigger_at, const std::string& commands )
{
  // TODO: object_q_mutex
  logger.sketch("AT","%lu %s",trigger_at,commands.c_str());
  object_q.push(SchedulableObject(trigger_at,commands));
  cerr << "posting..." << endl;
  sem_post(&sem);
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
