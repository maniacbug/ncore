#ifndef __SPI_QUEUE_H__
#define __SPI_QUEUE_H__

#include <queue>
#include <string>

#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>

#include <IDispatchable.h>

/**
 * Threadsafe blocking queue.  Empty queue blocks the thread until data is
 * available.
 *
 * @todo Move into its own header file.
 */

template <class T>
class QueueTS
{
private:
  std::queue<T> q;
  pthread_mutex_t mutex;
  sem_t sem;
public:
  QueueTS(void);
  virtual ~QueueTS();
  void push(const T&);
  T pop(void);
  void clear(void);
  bool available(void) const;
};

template <class T>
QueueTS<T>::QueueTS(void)
{
  pthread_mutex_init(&mutex,NULL);
  sem_init(&sem,0,0);
}

template <class T>
QueueTS<T>::~QueueTS()
{
  pthread_mutex_destroy(&mutex);
  sem_destroy(&sem);
}

template <class T>
void QueueTS<T>::push(const T& item)
{
  pthread_mutex_lock(&mutex);
  q.push(item);
  sem_post(&sem);
  pthread_mutex_unlock(&mutex);
}

template <class T>
T QueueTS<T>::pop(void)
{
  sem_wait(&sem);
  pthread_mutex_lock(&mutex);
  T item = q.front();
  q.pop();
  pthread_mutex_unlock(&mutex);

  return item;
}

template <class T>
void QueueTS<T>::clear(void)
{
  while ( q.size() )
  {
    sem_wait(&sem);
    q.pop();
  }
}

template <class T>
bool QueueTS<T>::available(void) const
{
  int n;
  sem_getvalue(const_cast<sem_t*>(&sem),&n);
  return n > 0 ;
}

class Logger;

/**
 * Mimics Arduino SPI interface.  Provides a means to queue up SPI responses
 * and logs SPI transfers.
 */

class SpiQueue: public IDispatchable
{
private:
  QueueTS<uint8_t> qts;
  Logger& logger;
  bool has_default;
  uint8_t default_value;
protected:
  std::string& getCommands(void) const;
  bool runCommand( const Parser& );
  bool command_spi(const std::vector<std::string>&);
public:
  SpiQueue(Logger&);
  void hwEnqueue(uint8_t);
  uint8_t transfer(uint8_t);
  void clear(void);
};

#endif // __SPI_QUEUE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
