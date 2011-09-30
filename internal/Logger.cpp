// STL includes
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <numeric>
// C includes
#include <stdio.h>
#include <stdarg.h>
#include <regex.h>
// Library includes
#include <pthread.h>
// Project includes
#include <Dispatcher.h>
#include <Clock.h>
#include <Parser.h>
#include <Logger.h>

using namespace std;

/****************************************************************************/

extern "C" unsigned long millis(void);

/****************************************************************************/

//
// Rate throttler
//

static const int lines_per_check = 10;

void Logger::throttle_output_rate(void)
{
  if ( clock && ! --lines_remaining )
  {
    lines_remaining += lines_per_check;
    unsigned long now = clock->millis();
    if ( now - last_check < rate_limit )
    {
      unsigned long wait = rate_limit - (now - last_check);
      ostringstream warning;
      warning << "delay " << wait << " (rate limit)";
      add_message(string("IN LOG "),warning.str());
      clock->delay(wait);
    }
    last_check = clock->millis();
  }
}

/****************************************************************************/

struct count_contains
{
  string key;
  count_contains(const string& _key): key(_key) {}
  int operator()(int sum,const string& whole)
  {
    return sum + ( (whole.find(key) != std::string::npos )?1:0 );
  }
};

/****************************************************************************/

//
// Public interface
//

Logger::Logger(void): clock(NULL), last_check(0), rate_limit(default_rate_limit), lines_remaining(lines_per_check), verbose(false)
{
  pthread_mutex_init(&mutex,NULL);
}

/****************************************************************************/

Logger::Logger(Clock& _clock): clock(&_clock), last_check(0), rate_limit(default_rate_limit), lines_remaining(lines_per_check), verbose(false)
{
  pthread_mutex_init(&mutex,NULL);
}

/****************************************************************************/

Logger::~Logger()
{
  pthread_mutex_destroy(&mutex);
}

/****************************************************************************/

void Logger::clear(void)
{
  vector<string>::clear();
  last_check = 0;
  lines_remaining = lines_per_check;
  pthread_mutex_destroy(&mutex);
  pthread_mutex_init(&mutex,NULL);
  rate_limit = default_rate_limit;
  verbose = false;
}

/****************************************************************************/

int Logger::lines_contain(const std::string& value) const
{
  return accumulate(begin(),end(),0,count_contains(value));
}

/****************************************************************************/

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

/****************************************************************************/

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

/****************************************************************************/

void Logger::sketch_v(const std::string& module, const std::string& format, va_list ap)
{
  pthread_mutex_lock( &mutex );

  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);

  string preamble = string("SK ") + module + "    ";
  preamble.resize(7);
  add_buffer(preamble);

  pthread_mutex_unlock( &mutex );

  throttle_output_rate();
}

/****************************************************************************/

void Logger::add_message(const std::string& preamble,const std::string& message)
{
  ostringstream ss;
  ss << "NCORE: ";
  if (clock)
    ss << setfill('0') << setw(6) << clock->millis() << " ";
  if (preamble.size())
    ss << preamble << " ";
  ss << message << endl;

  if ( verbose )
    cout << ">" << ss.str() ;

  push_back(ss.str());
}

/****************************************************************************/

void Logger::add_buffer(const std::string& preamble)
{
  add_message(preamble,string(buffer));
}

/****************************************************************************/

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

/****************************************************************************/

bool Logger::runCommand( const Parser& parser )
{
  bool result = false;

  const string& command = parser.at(0);

  if ( command == "list" )
  {
    result = command_list(parser);
  }
  else if ( command == "log" )
  {
    result = command_log(parser);
  }
  else if ( command == "help" )
  {
    const string& command = parser.at(1);

    if ( command == "list" )
    {
      cout << "list -- list all log entries from beginning" << endl;
      cout << "list /regexp/ -- list all log entries that match the regexp" << endl;
    }
    else if ( command == "log" )
    {
      cout << "log rate <xxx> -- set minimim time elapsed between 10 log entries" << endl;
    }
    result = true;
  }

  return result;
}

/****************************************************************************/

struct not_matches
{
  regex_t *regex;
  not_matches(const string& _key)
  {
    regex = (regex_t*) malloc(sizeof(regex_t));
    if ( regcomp(regex, _key.c_str(), REG_EXTENDED) )
      throw new runtime_error( string("Error in regular expression ") + _key );
  }
  ~not_matches()
  {
    regfree(regex);
  }
  bool operator()(const string& test) const
  {
    regmatch_t match;
    return regexec(regex, test.c_str(), 1, &match, 0);
  }
};

/****************************************************************************/

bool Logger::command_list(const vector<string>& _commands) const
{
  bool result = false;

  if ( _commands.size() == 1 )
  {
    pthread_mutex_lock( const_cast<pthread_mutex_t*>(&mutex) );
    copy(begin(),end(),ostream_iterator<string>(cout,""));
    pthread_mutex_unlock( const_cast<pthread_mutex_t*>(&mutex) );
    result = true;
  }
  else if ( _commands.size() == 2 )
  {
    string operand = _commands.at(1);
    if (operand.at(0) == '/' && operand.at(operand.size()-1) == '/')
    {
      string pattern = operand.substr(1,operand.size()-2);
      
      pthread_mutex_lock( const_cast<pthread_mutex_t*>(&mutex) );
      remove_copy_if(begin(),end(),ostream_iterator<string>(cout,""),not_matches(pattern));
      pthread_mutex_unlock( const_cast<pthread_mutex_t*>(&mutex) );

      result = true;
    }
    else
      throw new runtime_error("Expected regex as operand");
  }
  else
    throw new runtime_error("No parameters expected");

  return result;
}

/****************************************************************************/

bool Logger::command_log(const vector<string>& _commands)
{
  vector<string>::const_iterator current = _commands.begin() + 1;

  if ( current == _commands.end() )
    throw new runtime_error("Expecting operand");

  const string& operand = *current++;
  if ( operand == "rate" )
  {

    if ( current == _commands.end() )
      throw new runtime_error("Expecting rate value");

    istringstream ss(*current++);
    unsigned long rate;
    ss >> rate;

    internal("LOG","set rate limit to %ims",rate);

    pthread_mutex_lock( const_cast<pthread_mutex_t*>(&mutex) );
    rate_limit = rate;
    pthread_mutex_unlock( const_cast<pthread_mutex_t*>(&mutex) );
  }
  else if ( operand == "verbose" )
  {
    setVerbose(true);
    internal("LOG","set to verbose");
  }
  else if ( operand == "quiet" )
  {
    setVerbose(false);
    internal("LOG","set to quiet");
  }
  else
    throw new runtime_error("Unknown operand");

  return true;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
