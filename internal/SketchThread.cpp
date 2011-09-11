// STL includes
#include <stdexcept>
#include <iostream>
// C includes
// Library includes
#include <pthread.h>
// Project includes
#include <SketchThread.h>

/****************************************************************************/

extern "C" void setup(void);
extern "C" void loop(void);

/****************************************************************************/

SketchThread::SketchThread(int mode): pthread(NULL), custom_body(NULL), custom_body_wdata(NULL), custom_data(NULL)
{
  // Regular operation, controlling setup() and loop()
  if ( mode == 0)
  {
    pthread = new pthread_t;
    pthread_create( pthread, NULL, sketch_thread_main, NULL );
  }
  // Custom thread body, use SketchThread::startCustom to launch.
  else if ( mode == 1 )
  {
  }
  else
    throw new std::runtime_error("Unknown thread mode");
}

/****************************************************************************/

void SketchThread::startCustom(void (*fn)(void))
{
  if ( pthread )
    throw new std::runtime_error("Thread is already running");

  if ( !fn )
    throw new std::runtime_error("Must send a valid function");

  custom_body = fn;
  pthread = new pthread_t;
  pthread_create( pthread, NULL, sketch_thread_custom_main, this );
}

/****************************************************************************/

void SketchThread::startCustom(void (*fn)(void*),void* data)
{
  if ( pthread )
    throw new std::runtime_error("Thread is already running");

  if ( !fn )
    throw new std::runtime_error("Must send a valid function");

  if ( !data )
    throw new std::runtime_error("Must send a valid data pointer");

  custom_body_wdata = fn;
  custom_data = data;
  pthread = new pthread_t;
  pthread_create( pthread, NULL, sketch_thread_custom_wdata_main, this );
}

/****************************************************************************/

void* SketchThread::sketch_thread_custom_wdata_main(void* pv)
{
  SketchThread* psk = reinterpret_cast<SketchThread*>(pv);

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

  if ( psk && psk->custom_body_wdata )
    psk->custom_body_wdata(psk->custom_data);

  return NULL;
}

/****************************************************************************/

void* SketchThread::sketch_thread_custom_main(void* pv)
{
  SketchThread* psk = reinterpret_cast<SketchThread*>(pv);

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

  if ( psk && psk->custom_body )
    psk->custom_body();

  return NULL;
}

/****************************************************************************/

void* SketchThread::sketch_thread_main(void*)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

  setup();

  while(1)
    loop();

  return NULL;
}

/****************************************************************************/

void SketchThread::waitToFinish(void)
{
  pthread_join( *pthread, NULL );
}

/****************************************************************************/

SketchThread::~SketchThread(void)
{
  if (pthread)
  {
    pthread_cancel( *pthread );
    pthread_join( *pthread, NULL );
    delete pthread;
  }
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp

