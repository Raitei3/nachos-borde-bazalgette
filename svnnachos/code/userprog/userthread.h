
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

//static Lock *threadExit = new Lock("threadExit");
//static Lock *threadCreate = new Lock("threadCreate");
static Lock * threadExit = new Lock ("threadExit");



extern int do_ThreadCreate(int f, int arg,int arg2);
void StartUserThread(void * init);
void do_ThreadExit(int nbProcess);
void quit();


//#endif // USERTHREAD_H

#endif // CHANGED
