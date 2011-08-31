#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include <Logger.h>
#include <Dispatcher.h>

using namespace std;
extern "C" unsigned long millis(void);

static Logger* global_logger = NULL;

Logger::Logger(void)
{
  pthread_mutex_init(&mutex,NULL);
}

Logger::~Logger()
{
  pthread_mutex_destroy(&mutex);
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
  ss << "NCORE: " << setfill('0') << setw(6) << millis() << " " << buffer << endl;

  push_back(ss.str());
  pthread_mutex_unlock( &mutex );
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
  vector<string>::const_iterator current = begin();
  while ( current != end() )
  {
    cout << *current;
    ++current;
  }
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
