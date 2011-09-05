#ifndef __SKETCH_THREAD_H__
#define __SKETCH_THREAD_H__
#endif // __SKETCH_THREAD_H__

class SketchThread
{
private:
  pthread_t* pthread;
protected:
  static void* sketch_thread_main(void*);

public:
  SketchThread(int mode=0);
  void waitToFinish(void);
  virtual ~SketchThread();
};

// vim:cin:ai:sts=2 sw=2 ft=cpp
