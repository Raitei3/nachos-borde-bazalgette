
#ifdef CHANGED

#include "system.h"
#include "synch.h"
#include "bitmap.h"



//#ifndef USERTHREAD_H
//#define USERTHREAD_H


typedef struct argInitThread {
  int fun;
  int arg;
} argInitThread ;


extern int do_ThreadCreate(int f, int arg);
void StartUserThread(void * init);
void do_ThreadExit(Thread * t);


//#endif // USERTHREAD_H
#endif // CHANGED
