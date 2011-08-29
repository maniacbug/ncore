#include <sstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdarg.h>
#include <Logger.h>
#include <Dispatcher.h>

using namespace std;
extern "C" unsigned long millis(void);

static Logger* global_logger = NULL;

void Logger::add(const std::string& format,...)
{
  static char buffer[500];
  va_list ap;
  va_start(ap,format);
  vsnprintf(buffer,sizeof(buffer),format.c_str(),ap);
  va_end(ap);
  
  ostringstream ss;
  ss << "NCORE: " << millis() << " " << buffer << endl;

  push_back(ss.str());
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

  vector<string>::const_iterator current = begin();
  while ( current != end() )
  {
    cout << *current;
    ++current;
  }

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
