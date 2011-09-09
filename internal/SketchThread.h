#ifndef __SKETCH_THREAD_H__
#define __SKETCH_THREAD_H__

class SketchThread
{
private:
  pthread_t* pthread;
  void (*custom_body)(void);
  void (*custom_body_wdata)(void*);
  void *custom_data;
protected:
  static void* sketch_thread_main(void*);
  static void* sketch_thread_custom_main(void*);
  static void* sketch_thread_custom_wdata_main(void*);

public:
  SketchThread(int mode=0);
  void startCustom(void (*fn)(void));
  void startCustom(void (*fn)(void*),void*);
  void waitToFinish(void);
  virtual ~SketchThread();
};

#endif // __SKETCH_THREAD_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
