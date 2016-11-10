#ifdef CHANGED
#include "userthread.h"

Semaphore *threadCreate = new Semaphore("threadCreate",1);
Semaphore *startThread = new Semaphore("startThread",1);
Semaphore *threadExit = new Semaphore("threadExit",1);
Semaphore *test = new Semaphore("test",4);

int threadCountCreated = 1;
int threadCountDeleted = 0;

BitMap * bitmap;
int threadSlot;


int do_ThreadCreate(int f, int arg) {
  threadCreate->P();

  if (threadCountCreated == 1){
    bitmap = new BitMap(4);
    bitmap->Mark(0);
    currentThread->setIdMap(0);
    test->P();
    }
    threadCountCreated++;

  threadCreate->V();

  Thread * newThread = new Thread("User Thread");
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  init ->fun = f;
  init->arg = arg;

  newThread->Start(StartUserThread, init);
  //printf("test1");
  return 0;
}


  void StartUserThread(void * init) {
  int i;

  for (i = 0; i < NumTotalRegs; i++) {
      machine->WriteRegister (i, 0);
  }
  argInitThread* in = (argInitThread *) init;
  machine->WriteRegister (PCReg, in->fun);
  machine->WriteRegister (NextPCReg, machine -> ReadRegister(PCReg)+4);
  machine->WriteRegister (4, in->arg);
  free (init);


  test->P();

  //startThread->P();
  //while (bitmap->NumClear()==0) {currentThread->Yield();}

  if(bitmap -> NumClear() > 0){

    threadSlot = bitmap->Find();
    currentThread -> setIdMap(threadSlot);
    bitmap->Mark(threadSlot);

    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(threadSlot));
    }

  //startThread->V();
  machine->Run();
  //printf("test2");


  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
}


void do_ThreadExit(Thread * t){
threadExit->P();
  bitmap->Clear(t->getIdMap());
  threadCountDeleted++;
  test->V();
  if (threadCountDeleted == threadCountCreated){
    delete bitmap;
    delete threadExit;
    delete threadCreate;
    delete startThread;
    interrupt->Halt();
    }
  threadExit->V();
  t->Finish();

}
#endif
