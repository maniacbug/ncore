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
 */

template <class T> 
class QueueTS
{
private:
  std::queue<T> q; /**< The queue of objects */
  pthread_mutex_t mutex; /**< A mutex controlling access to the queue */
  pthread_cond_t cond; /**< Condition var which allows one caller to block waiting for another caller to insert */
public:
  /**
   * Default constructor
   */
  QueueTS(void);

  /**
   * Destructor
   */
  virtual ~QueueTS();
  
  /**
   * Push an object onto the queue
   *
   * @param object Object to be copied into the queue
   */
  void push(const T& object);
  
  /**
   * Pop an object off the queue and return it
   *
   * @return Object popped off queue
   */
  T pop(void);
  
  /**
   * Clears the queue to zero objects
   */
  void clear(void);
  
  /**
   * Test whether there is an object to be popped.  If this returns false,
   * you can expect a call to pop() is going to block.
   *
   * @return Whether there is at least one object on the queue.
   */
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
