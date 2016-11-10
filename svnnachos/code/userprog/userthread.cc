#ifdef CHANGED
#include "userthread.h"
#include "scheduler.h"

static Lock *threadExit = new Lock("threadExit");
static Semaphore *execThreadSector = new Semaphore("execThreadSector",4);

static BitMap * bitmap = NULL;
//------------------------------------------------------------------

int do_ThreadCreate(int f, int arg) {
  if (bitmap == NULL){
    initFirstThread();
  }
  Thread * newThread = new Thread("User Thread");
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  init ->fun = f;
  init->arg = arg;
  newThread->Start(StartUserThread, init);
  return 0;
}


void StartUserThread(void * init) {
  for (int i = 0; i < NumTotalRegs; i++) {
    machine->WriteRegister (i, 0);
  }
  argInitThread* in = (argInitThread *) init;
  machine->WriteRegister (PCReg, in->fun);
  machine->WriteRegister (NextPCReg, machine -> ReadRegister(PCReg)+4);
  machine->WriteRegister (4, in->arg);
  free (init);

  execThreadSector->P();
  int threadSlot = bitmap->Find();
  currentThread -> setIdMap(threadSlot);
  bitmap->Mark(threadSlot);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(threadSlot));
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  machine->Run();
}


void do_ThreadExit(Thread * t){
  threadExit->Acquire();
  bitmap->Clear(t->getIdMap());
  if (bitmap->NumClear() == 4 && scheduler->FindNextToRun() == NULL){
    quit();
  }
  threadExit->Release();
  execThreadSector->V();
  t->Finish();
}


void initFirstThread(){
  bitmap = new BitMap(4);
  bitmap->Mark(0);
  currentThread->setIdMap(0);
  execThreadSector->P();
}


void quit(){
  delete bitmap;
  delete threadExit;
  interrupt->Halt();
}


#endif
