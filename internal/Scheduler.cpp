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

#if MAC

#include <mach/clock.h>
#include <mach/mach.h>

// https://gist.github.com/1087739
static void clock_gettime(uint8_t, timespec* ts)
{
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
}
static const int CLOCK_REALTIME = 1;
#endif

/****************************************************************************/

Scheduler::Scheduler(Dispatcher& _d, Logger& _l): dispatch(_d), logger(_l), done(false)
{
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,0);
}

/****************************************************************************/

Scheduler::~Scheduler()
{
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

/****************************************************************************/

void Scheduler::runonce(void)
{
  pthread_mutex_lock(&mutex);
  while ( object_q.empty() )
  {
    pthread_cond_wait(&cond,&mutex);
  }
    unsigned long top_trigger_at = object_q.top().trigger_at;
    unsigned long now = clock.millis();
    if ( now >= top_trigger_at )
    {
      SchedulableObject o = object_q.top();
      object_q.pop();
      pthread_mutex_unlock(&mutex);

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
      pthread_cond_timedwait(&cond,&mutex,&tm);
      pthread_mutex_unlock(&mutex);
    }
}

/****************************************************************************/

void Scheduler::add(unsigned long trigger_at, const std::string& commands )
{
  logger.sketch("AT","%lu %s",trigger_at,commands.c_str());
  pthread_mutex_lock(&mutex);
  object_q.push(SchedulableObject(trigger_at,commands));
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&cond);
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
  pthread_mutex_lock(&mutex);
  while ( ! object_q.empty() )
    object_q.pop();
  pthread_mutex_unlock(&mutex);
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
