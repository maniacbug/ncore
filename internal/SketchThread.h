
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SKETCH_THREAD_H__
#define __SKETCH_THREAD_H__

/**
 * Encapsulates the thread used to run an Arduino sketch.  Also provides a
 * general-purpose thread interface for other uses.
 *
 * @todo Should split the general-purpose thread out into its own lower-
 * level class.
 */

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
  static void* sketch_thread_inherited_main(void*);

  virtual void internal_setup(void) {}
  virtual void internal_loop(void) {}

public:
  SketchThread(int mode=0);
  void startCustom(void (*fn)(void));
  void startCustom(void (*fn)(void*),void*);
  void waitToFinish(void);
  virtual ~SketchThread();
};

/**
 * Implements a system where multiple threads can co-exist but only one can
 * run.  Each thread has an ID, and can yield control to another thread's ID/
 */

class ThreadDeconflict
{
private:
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  volatile int current_id;
public:
  ThreadDeconflict();
  ~ThreadDeconflict();
  void yieldTo(int to);
  void wait(int from);
};

#endif // __SKETCH_THREAD_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
