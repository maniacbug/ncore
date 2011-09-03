#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include <Logger.h>
#include <Dispatcher.h>
#include <Clock.h>
#include <Parser.h>

using namespace std;
extern "C" unsigned long millis(void);

static Logger* global_logger = NULL;

//
// Static rate throttler
//

static const unsigned long min_interval = 500; // ms
static const int lines_per_check = 10;

void Logger::throttle_output_rate(void)
{
  if ( clock && ! --lines_remaining )
  {
    lines_remaining += lines_per_check;
    unsigned long now = clock->millis();
    if ( now - last_check < min_interval )
    {
      unsigned long wait = min_interval - (now - last_check); 
      ostringstream warning;
      warning << "Rate limiter: delay " << wait << "ms" << endl;
      push_back(warning.str());
      clock->delay(wait);
    }
    last_check = clock->millis();
  }
}

//
// Public interface
//

Logger::Logger(void): clock(NULL), last_check(0), lines_remaining(lines_per_check)
{
  pthread_mutex_init(&mutex,NULL);
}

Logger::~Logger()
{
  pthread_mutex_destroy(&mutex);
}

void Logger::clear(void)
{
  vector<string>::clear();
  last_check = 0;
  lines_remaining = lines_per_check;
}

void Logger::add(const std::string& format,...)
{
  pthread_mutex_lock( &mutex );

  static char buffer[500];
  va_list ap;
  va_start(ap,format);
  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);
  va_end(ap);

  ostringstream ss;
  ss << "NCORE: ";
  if (clock)
    ss << setfill('0') << setw(6) << clock->millis() << " ";
  ss << buffer << endl;

  push_back(ss.str());
  pthread_mutex_unlock( &mutex );

  throttle_output_rate();
}

void Logger::setClock(const Clock& _clock)
{
  clock = &_clock;
}

bool Logger::runCommand( const Parser& parser ) 
{ 
  bool result = false;
  
  const string& command = parser.at(0);

  if ( command == "list" )
  {
    result = command_list(parser);
  }

  return result; 
}

bool Logger::static_command_list(const vector<string>& _commands)
{
  if ( ! global_logger )
    throw new runtime_error("No logger registered to receive commands");

  return global_logger->command_list(_commands);
}

bool Logger::command_list(const vector<string>& _commands) const
{
  if ( _commands.size() != 1 )
    throw new runtime_error("No parameters expected");

  pthread_mutex_lock( const_cast<pthread_mutex_t*>(&mutex) );
  copy(begin(),end(),ostream_iterator<string>(cout,""));
  pthread_mutex_unlock( const_cast<pthread_mutex_t*>(&mutex) );

  return true;
}

void Logger::addCommandsTo(Dispatcher& _commands)
{
  global_logger = this;
  _commands.add("list",Logger::static_command_list);
}

void Logger::reset(void)
{
  global_logger = NULL;
}
#if 0

void logger::add(const char* format,...)
{
  static header[100];
  snprintf(header,sizeof(header),"NCORE: %lu ",millis());
  static buffer[500];
  vsnprintf(buffer,sizeof(buffer),format,va_args);
  add(string(header)+string(buffer));
}

void logger::add(unsigned long ms, const char* format,...)
{
  static header[100];
  snprintf(header,sizeof(header),"NCORE: %lu ",ms);
  static buffer[500];
  vsnprintf(buffer,sizeof(buffer),format,va_args);
  add(string(header)+string(buffer));
}
#endif
// vim:cin:ai:sts=2 sw=2 ft=cpp
