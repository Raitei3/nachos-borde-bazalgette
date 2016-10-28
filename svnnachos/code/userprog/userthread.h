
#ifdef CHANGED

#include "../threads/thread.h"
#include "../machine/machine.h"
#include "addrspace.h"
#include "system.h"
#include "syscall.h"
#include "copyright.h"


#ifndef USERTHREAD_H
#define USERTHREAD_H

typedef struct argInitThread {
  int fun;
  int* args;
} argInitThread ;


extern int do_Thread_Create(int f, int arg);
static void StartUserThread(void * init);
void do_ThreadExit(Thread * t);


#endif // USERTHREAD_H
#endif // CHANGED
