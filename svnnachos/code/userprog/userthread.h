
#ifdef CHANGED

#include "system.h"
#include "synch.h"
#include "bitmap.h"



//#ifndef USERTHREAD_H
//#define USERTHREAD_H


typedef struct argInitThread {
  int fun;
  int arg;
  int addrThreadExit;
} argInitThread ;


extern int do_ThreadCreate(int f, int arg,int arg2,int nbThreadCreated);
void StartUserThread(void * init);
void do_ThreadExit();
void quit();


//#endif // USERTHREAD_H
#endif // CHANGED
