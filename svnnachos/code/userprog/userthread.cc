#ifdef CHANGED
#include "userthread.h"
#include "scheduler.h"
#include "syscall.h"

static Lock *threadExit = new Lock("threadExit");
static Semaphore *execThreadSector = new Semaphore("execThreadSector",3);
static Lock *threadCreate = new Lock("threadCreate");

static BitMap * bitmap = NULL;
static char s[255];

//------------------------------------------------------------------

int do_ThreadCreate(int f, int arg,int addr,int nbThreadCreated) {
  threadCreate -> Acquire(); //on protège avec un lock.
  //peut servir si on créé des threads en cascade.

  if (bitmap == NULL){  // on regarde si la bitmap est instanciée et si ce
    initFirstThread();  // n'est pas le cas on sait qu'on doit tout initialiser.
  }
  sprintf(s,"UserTread-%d",nbThreadCreated); //on genère le nom du thread en utilisant nbThreadCreated

  Thread * newThread = new Thread(s);
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  if (init==NULL || newThread==NULL){
    return -1;
  }

  init ->fun = f;
  init->arg = arg;
  init->addrThreadExit = addr;
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
  machine->WriteRegister(31, in ->addrThreadExit); //on met l'adresse de threadExit
                                                   //dans le registre de retour.
  free (init);

  execThreadSector->P();  //Le Semaphore principal qui empèche + de 4 threads d'être actifs en meme temps

  int threadSlot = bitmap->Find();
  currentThread -> setIdMap(threadSlot);
  bitmap->Mark(threadSlot);
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(threadSlot));
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  machine->Run();
}


void do_ThreadExit(){

  threadExit->Acquire();  //on protege la bitmap d'éventuel accé concurrent
  bitmap->Clear(currentThread->getIdMap());
  threadExit->Release();

  execThreadSector->V();  //on rend un jeton : la place est desormais libre
  currentThread->Finish();
}


//initialise le main et la bitmap.
void initFirstThread(){
  bitmap = new BitMap(4);
  bitmap->Mark(0);
  currentThread->setIdMap(0);
}


// Libère les objets créés
void quit(){
  delete bitmap;
  delete threadExit;
  delete threadCreate;
  printf("\n");
}


#endif
