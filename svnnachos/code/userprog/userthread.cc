#include "userthread.h"

int threadCountCreated=1;
int threadCountDeleted=0;
int stackCountAllocate=1;

static void StartUserThread(void * init);

int do_ThreadCreate(int f, int arg) {
  threadCountCreated+=1;
  Thread * newThread = new Thread("User Thread");

  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  init ->fun = f;
  init->arg = arg;
  newThread->Start(StartUserThread, init);
  return 0;
}

static void StartUserThread(void * init) {
  int i;

  for (i = 0; i < NumTotalRegs; i++) {
      machine->WriteRegister (i, 0);
  }
  argInitThread* in = (argInitThread *) init;
  machine->WriteRegister (PCReg, in->fun);
  machine->WriteRegister (NextPCReg, machine -> ReadRegister(PCReg)+4);
  machine->WriteRegister (4, in->arg);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(stackCountAllocate));
  stackCountAllocate +=1;
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  machine->Run();
}

void do_ThreadExit(Thread * t){
  threadCountDeleted+=1;
  //printf("delete:%d,%d\n",threadCountCreated,threadCountDeleted);
  if (threadCountDeleted == threadCountCreated)
    interrupt->Halt();
  t->Finish();
}
