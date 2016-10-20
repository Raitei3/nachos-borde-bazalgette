#include "userthread.h"
#include "../threads/thread.h"
#include "../machine/machine.h"

typedef struct argInitThread {
  int fun;
  int* args;
} argInitThread;

int do_ThreadCreate(int f, int arg) {
  Thread * newThread = new Thread("Thread utilisateur");
  argInitThread init = malloc(sizeof(argInitThread));
  init->fun = f;
  init->args = &arg;
  //newThread->Start(StartUserThread, init);
  
  return 0;
}

static void StartUserThread(argInitThread init) {
  int i;
  for (i = 0; i < NumTotalRegs; i++) {
      machine->WriteRegister (i, 0);
  }
  machine->WriteRegister (PCReg, init->fun);
  machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister (StackReg, AddrSpace->AllocateUserThread());
  DEBUG ('a', "Initializing stack register to 0x%x\n", numPages * PageSize - (256 + 16*2);
  
