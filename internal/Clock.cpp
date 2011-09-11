// STL includes
#include <cstdlib>
// C includes
#include <unistd.h>
// Library includes
// Project includes
#include <Clock.h>

/****************************************************************************/

Clock::Clock(void)
{
  gettimeofday(&program_start,NULL);
}

/****************************************************************************/

void Clock::delay(unsigned long ms) const
{
  usleep(ms*1000LU);
}

/****************************************************************************/

unsigned long Clock::millis(void) const
{
  unsigned long result;
  struct timeval now;
  gettimeofday(&now,NULL);

  result = ( now.tv_sec - program_start.tv_sec ) * 1000LU ;
  result += ( now.tv_usec - program_start.tv_usec ) / 1000L ;

  return result;
}

/****************************************************************************/

void Clock::delayMicroseconds(unsigned int us) const
{
  usleep(us);
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2
