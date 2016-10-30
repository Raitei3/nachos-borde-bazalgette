#include "userthread.h"
#include "../threads/scheduler.h"


static void StartUserThread(void * init);

int do_ThreadCreate(int f, int arg) {
  Thread * newThread = new Thread("User Thread");
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  init ->fun = f;
  printf("do_ThreadCreate : %p\n",init -> fun );
  init-> args = &arg;
  newThread->Start(StartUserThread, init);
  scheduler -> Run(newThread);

  return 0;
}

static void StartUserThread(void * init) {
  /*int i;
  for (i = 0; i < NumTotalRegs; i++) {
      machine->WriteRegister (i, 0);
  }*/
  argInitThread* in = (argInitThread *) init; // a demander au prof
  printf("StartUserThread : %p\n", in -> fun );
  machine->WriteRegister (PCReg, in->fun);
  machine->WriteRegister (NextPCReg, machine -> ReadRegister(PCReg)+4);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack());
  DEBUG ('a', "Initializing stack register to 0x%x\n");
}

void do_ThreadExit(Thread * t){
  t->Finish();
}
