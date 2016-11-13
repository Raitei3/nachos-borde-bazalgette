#ifdef CHANGED
#include "userthread.h"
#include "scheduler.h"
#include "syscall.h"

static Lock *threadExit = new Lock("threadExit");
static Semaphore *execThreadSector = new Semaphore("execThreadSector",3);
static Lock *threadCreate = new Lock("threadCreate");

static BitMap * bitmap = NULL;
static char s[255];

int nbThreadCreated=0;
//------------------------------------------------------------------

int do_ThreadCreate(int f, int arg,int arg2) {
  threadCreate -> Acquire();
  if (bitmap == NULL){
    initFirstThread();
  }
  nbThreadCreated++;
  sprintf(s,"UserTread-%d",nbThreadCreated);

  Thread * newThread = new Thread(s);
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  if (init==NULL || newThread==NULL){
    return -1;
  }
  init ->fun = f;
  init->arg = arg;
  init->arg2 = arg2;
  newThread->Start(StartUserThread, init);
  threadCreate -> Release();
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
  machine->WriteRegister(31, in ->arg2);
  free (init);

  execThreadSector->P();
  int threadSlot = bitmap->Find();
  currentThread -> setIdMap(threadSlot);
  bitmap->Mark(threadSlot);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(threadSlot));
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  machine->Run();
}


void do_ThreadExit(){

  threadExit->Acquire();
  bitmap->Clear(currentThread->getIdMap());
  threadExit->Release();
  execThreadSector->V();
  currentThread->Finish();
}


void initFirstThread(){
  bitmap = new BitMap(4);
  bitmap->Mark(0);
  currentThread->setIdMap(0);
}


void quit(){
  delete bitmap;
  delete threadExit;
  delete threadCreate;
  printf("\n");
}


#endif
