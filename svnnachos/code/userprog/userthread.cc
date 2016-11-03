#include "userthread.h"

static void StartUserThread(void * init);

int do_ThreadCreate(int f, int arg) {
  Thread * newThread = new Thread("User Thread");
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  init ->fun = f;
  init-> args = &arg;
  newThread->Start(StartUserThread, init);
  return 0;
}

static void StartUserThread(void * init) {
  int i;
  for (i = 0; i < NumTotalRegs; i++) {
      machine->WriteRegister (i, 0);
  }
  argInitThread* in = (argInitThread *) init; // a demander au prof
  machine->WriteRegister (PCReg, in->fun);
  machine->WriteRegister (NextPCReg, machine -> ReadRegister(PCReg)+4);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack());
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  machine->Run();
}

void do_ThreadExit(Thread * t){
  t->Finish();
}
