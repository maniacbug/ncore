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
#include <Parser.h>

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
    sem_wait(&sem);
    logger.sketch("AT","Got sem");
  }
  if ( ! object_q.empty() )
  {
    // TODO: pthread_mutex_lock object_q_mutex
    unsigned long top_trigger_at = object_q.top().trigger_at;
    unsigned long now = clock.millis();
    if ( now >= top_trigger_at )
    {
      SchedulableObject o = object_q.top();
      object_q.pop();
      // TODO: pthread_mutex_unlock object_q_mutex

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
      // TODO: pthread_mutex_unlock object_q_mutex
      
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
  logger.sketch("AT","%lu %s",trigger_at,commands.c_str());
  // TODO: pthread_mutex_lock object_q_mutex
  object_q.push(SchedulableObject(trigger_at,commands));
  // TODO: pthread_mutex_unlock object_q_mutex
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
  // TODO: pthread_mutex_lock object_q_mutex
  while ( ! object_q.empty() )
    object_q.pop();
  // TODO: pthread_mutex_unlock object_q_mutex
}

string& Scheduler::getCommands(void) const 
{ 
  static std::string commands = "at"; 
  return commands; 
}

bool Scheduler::runCommand( const Parser& parser ) 
{ 
  bool result = false;
  
  const string& command = parser.at(0);

  if ( command == "at" )
  {
    result = true;
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);
    if ( command == "at" )
    {
      cout << "at <t> <commands> -- run <commands> at time <t> ms." << endl;
    }
    result = true;
  }

  return result; 
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
