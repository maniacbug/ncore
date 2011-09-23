#ifndef __QUEUETS_H__
#define __QUEUETS_H__

// STL includes
#include <queue>
// C includes
#include <pthread.h>
// Library includes
// Project includes

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
  pthread_cond_t cond;
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
  pthread_cond_init(&cond,0);
}
  
template <class T> 
QueueTS<T>::~QueueTS()
{
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

template <class T> 
void QueueTS<T>::push(const T& item)
{
  pthread_mutex_lock(&mutex);
  q.push(item);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&cond);
}

template <class T> 
T QueueTS<T>::pop(void)
{
  pthread_mutex_lock(&mutex);
  while ( !q.size() )
    pthread_cond_wait(&cond,&mutex);
  T item = q.front();
  q.pop();
  pthread_mutex_unlock(&mutex);

  return item;
}
  
template <class T> 
void QueueTS<T>::clear(void)
{
  pthread_mutex_lock(&mutex);
  while ( q.size() )
    q.pop();
  pthread_mutex_unlock(&mutex);
}

template <class T> 
bool QueueTS<T>::available(void) const
{
  pthread_mutex_lock(const_cast<pthread_mutex_t*>(&mutex));
  bool result = q.size() > 0;
  pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mutex));
  return result;
}

#endif // __QUEUETS_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
