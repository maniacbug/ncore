#include <pthread.h>

#include <SketchThread.h>

extern "C" void setup(void);
extern "C" void loop(void);

SketchThread::SketchThread(void)
{
  pthread = new pthread_t;
  pthread_create( pthread, NULL, sketch_thread_main, NULL );
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

