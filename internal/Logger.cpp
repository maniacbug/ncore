#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <numeric>

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include <Logger.h>
#include <Dispatcher.h>
#include <Clock.h>
#include <Parser.h>

using namespace std;

extern "C" unsigned long millis(void);

//
// Rate throttler
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
      warning << "delay " << wait << " (rate limit)";
      add_message(string("IN LOG "),warning.str());
      clock->delay(wait);
    }
    last_check = clock->millis();
  }
}

struct count_contains
{
  string key;
  count_contains(const string& _key): key(_key) {}
  int operator()(int sum,const string& whole)
  {
    return sum + ( (whole.find(key) != std::string::npos )?1:0 );
  }
};

//
// Public interface
//

Logger::Logger(void): clock(NULL), last_check(0), lines_remaining(lines_per_check)
{
  pthread_mutex_init(&mutex,NULL);
}

Logger::Logger(Clock& _clock): clock(&_clock), last_check(0), lines_remaining(lines_per_check)
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
  pthread_mutex_destroy(&mutex);
  pthread_mutex_init(&mutex,NULL);
}

int Logger::lines_contain(const std::string& value) const
{
  return accumulate(begin(),end(),0,count_contains(value));
}

void Logger::internal(const std::string& module, const std::string& format,...)
{
  pthread_mutex_lock( &mutex );

  va_list ap;
  va_start(ap,format);
  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);
  va_end(ap);

  string preamble = string("IN ") + module + "    ";
  preamble.resize(7);
  add_buffer(preamble);

  pthread_mutex_unlock( &mutex );
}

void Logger::sketch(const std::string& module, const std::string& format,...)
{
  pthread_mutex_lock( &mutex );

  va_list ap;
  va_start(ap,format);
  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);
  va_end(ap);
 
  string preamble = string("SK ") + module + "    ";
  preamble.resize(7);
  add_buffer(preamble);

  pthread_mutex_unlock( &mutex );

  throttle_output_rate();
}

void Logger::add_message(const std::string& preamble,const std::string& message)
{
  ostringstream ss;
  ss << "NCORE: ";
  if (clock)
    ss << setfill('0') << setw(6) << clock->millis() << " ";
  if (preamble.size())
    ss << preamble << " ";
  ss << message << endl;

  push_back(ss.str());
}

void Logger::add_buffer(const std::string& preamble)
{
  add_message(preamble,string(buffer));
}

void Logger::add(const std::string& format,...)
{
  pthread_mutex_lock( &mutex );

  va_list ap;
  va_start(ap,format);
  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);
  va_end(ap);
  
  add_buffer(string());

  pthread_mutex_unlock( &mutex );

  throttle_output_rate();
}

bool Logger::runCommand( const Parser& parser ) 
{ 
  bool result = false;
  
  const string& command = parser.at(0);

  if ( command == "list" )
  {
    result = command_list(parser);
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);

    if ( command == "list" )
    {
      cout << "list -- list all log entries from beginning" << endl;
    }
    result = true;
  }

  return result; 
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

// vim:cin:ai:sts=2 sw=2 ft=cpp
