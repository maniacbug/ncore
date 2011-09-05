#ifndef __SPI_QUEUE_H__
#define __SPI_QUEUE_H__

#include <queue>
#include <string>
#include <pthread.h>
#include <semaphore.h>

class Logger;

// Threadsafe blocking queue.  "Pop" will block if the queue is empty.

class QueueTS
{
private:
  std::queue<std::string> q;
  pthread_mutex_t mutex;
  sem_t sem;
  Logger& logger;
public:
  QueueTS(Logger&);
  virtual ~QueueTS();
  void push(const std::string&);
  std::string pop(void);
  void clear(void);
};
#endif // __SPI_QUEUE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
