#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <Logger.h>

using namespace std;
extern "C" unsigned long millis(void);

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
