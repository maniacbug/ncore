#include <unistd.h>
#include <sys/time.h>
#include <string.h>

struct timeval mock_time = {0,0};

extern "C" {

  void mock_time_reset(void)
  {
    memset(&mock_time,0,sizeof(mock_time));
  }

  int usleep(useconds_t us)
  {
    mock_time.tv_usec += us;
    while ( mock_time.tv_usec >= 1000000L )
    {
      mock_time.tv_sec ++;
      mock_time.tv_usec -= 1000000L;
    }
    return 0;
  }

  int gettimeofday(struct timeval *tv, struct timezone *)
  {
    memcpy(tv,&mock_time,sizeof(mock_time));
    return 0;
  }

  time_t time(time_t *p)
  {
    if (p)
      *p = mock_time.tv_sec;
    return mock_time.tv_sec;
  }

}

