// STL includes
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <sstream>
// C includes
#include <time.h>
// Library includes
// Project includes
#include <Dispatcher.h>
#include <Logger.h>
#include <Parser.h>
#include <Scheduler.h>

/****************************************************************************/

using namespace std;

/****************************************************************************/

Scheduler::Scheduler(Dispatcher& _d, Logger& _l): dispatch(_d), logger(_l), done(false)
{
  sem_init(&sem,0,0);
}

/****************************************************************************/

Scheduler::~Scheduler()
{
  sem_destroy(&sem);
}

/****************************************************************************/

void Scheduler::runonce(void)
{
  if ( object_q.empty() )
  {
    sem_wait(&sem);
#if 0
    logger.sketch("AT","Got sem");
#endif 
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

      if ( o.commands == "quit" )
        done = true;

      try
      {
        dispatch.execute(o.commands);
      }
      catch (runtime_error* e)
      {
        logger.sketch("AT", "Error: %s",e->what());
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
#if 0
      logger.sketch("AT","Waiting %lu",wait);
      int result = sem_timedwait(&sem,&tm);
      // This is debugging info, not really needed for regular use.
      if ( result )
        logger.sketch("AT","Sem timeout");
      else
        logger.sketch("AT","Got sem");
#else
      sem_timedwait(&sem,&tm);
#endif
    }
  }
}

/****************************************************************************/

void Scheduler::add(unsigned long trigger_at, const std::string& commands )
{
  logger.sketch("AT","%lu %s",trigger_at,commands.c_str());
  // TODO: pthread_mutex_lock object_q_mutex
  object_q.push(SchedulableObject(trigger_at,commands));
  // TODO: pthread_mutex_unlock object_q_mutex
  sem_post(&sem);
}

/****************************************************************************/

size_t Scheduler::size(void) const
{
  return object_q.size();
}

/****************************************************************************/

void Scheduler::clear(void)
{
  clock = Clock();
  // TODO: pthread_mutex_lock object_q_mutex
  while ( ! object_q.empty() )
    object_q.pop();
  // TODO: pthread_mutex_unlock object_q_mutex
}

/****************************************************************************/

string& Scheduler::getCommands(void) const
{
  static std::string commands = "at";
  return commands;
}

/****************************************************************************/

bool Scheduler::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "at" )
  {
    result = command_at(parser);
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

/****************************************************************************/

bool Scheduler::command_at(const vector<string>& _commands)
{
  if ( _commands.size() < 3 )
    throw new runtime_error("Usage: at <x> <commands>");

  int trigger_at;
  istringstream convert(_commands.at(1));
  convert >> dec >> trigger_at;


  ostringstream commandstr;
  copy(_commands.begin()+2,_commands.end(),ostream_iterator<string>(commandstr," "));
  add(trigger_at,commandstr.str());

  return true;
}

/****************************************************************************/

void Scheduler::handler_thread_main(void* pv)
{
  Scheduler* psched = reinterpret_cast<Scheduler*>(pv);

  while(!psched->done)
    psched->runonce();
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
