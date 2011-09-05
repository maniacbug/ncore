#include <queue>
#include <string>

#include <pthread.h>
#include <semaphore.h>

#include <Logger.h>
#include <SpiQueue.h>

using namespace std;

QueueTS::QueueTS(Logger& _logger): logger(_logger)
{
  pthread_mutex_init(&mutex,NULL);
  sem_init(&sem,0,0);
}
  
QueueTS::~QueueTS()
{
  pthread_mutex_destroy(&mutex);
  sem_destroy(&sem);
}

void QueueTS::push(const string& _str)
{
  pthread_mutex_lock(&mutex);
  q.push(_str);
  sem_post(&sem);
  pthread_mutex_unlock(&mutex);
}

string QueueTS::pop(void)
{
  int n;
  sem_getvalue(&sem,&n);
  if ( ! n )
    logger.sketch("QPOP","Blocking...");
  sem_wait(&sem);
  pthread_mutex_lock(&mutex);
  string s = q.front();
  q.pop();
  pthread_mutex_unlock(&mutex);

  return s;
}
  
void QueueTS::clear(void)
{
  while ( q.size() )
  {
    sem_wait(&sem);
    q.pop();
  }
}

bool QueueTS::available(void) const
{
  int n;
  sem_getvalue(const_cast<sem_t*>(&sem),&n);
  return n > 0 ; 
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
