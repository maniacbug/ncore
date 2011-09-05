#include <stdexcept>
#include <iostream>

#include <pthread.h>
#include <SketchThread.h>

extern "C" void setup(void);
extern "C" void loop(void);

SketchThread::SketchThread(int mode): pthread(NULL)
{

  // Regular operation, controlling setup() and loop()
  if ( mode == 0)
  {
    pthread = new pthread_t;
    pthread_create( pthread, NULL, sketch_thread_main, NULL );
  }
  else
    throw new std::runtime_error("Unknown thread mode");
}

void* SketchThread::sketch_thread_main(void*)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

  setup();

  while(1)
    loop();

  return NULL;
}

void SketchThread::waitToFinish(void)
{
  pthread_join( *pthread, NULL );
}

SketchThread::~SketchThread(void)
{
  pthread_cancel( *pthread );
  pthread_join( *pthread, NULL );
  delete pthread;
}
// vim:cin:ai:sts=2 sw=2 ft=cpp

