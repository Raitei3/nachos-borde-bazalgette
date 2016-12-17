#ifdef CHANGED
#include "userthread.h"
#include "scheduler.h"
#include "syscall.h"



//------------------------------------------------------------------
int nbThreadCreated = 0;

int do_ThreadCreate(int f, int arg,int addr) {



  if (currentThread->space->threadBitmap == NULL){  // on regarde si la bitmap est instanciée et si ce
    currentThread->space->initFirstThread();  // n'est pas le cas on sait qu'on doit tout initialiser.
    currentThread->space->execThreadSector = new Semaphore("execThreadSector",NBTHREAD-1);
    currentThread->space->threadCreate = new Lock("threadCreate");
  }
  currentThread->space->execThreadSector->P();
  currentThread->space->threadCreate -> Acquire(); //on protège avec un lock.
  //peut servir si on créé des threads en cascade.
  //printf("%s\n","threadcreate" );
  //nbThreadCreated++;
  //char * s =(char*) malloc(sizeof(char)*80);  sprintf(s,"%s : UserTread-%d",currentThread->getName(),nbThreadCreated); //on genère le nom du thread en utilisant nbThreadCreated
  char s[50] = "thread-noyau";

  Thread * newThread = new Thread(s);
  argInitThread * init = (argInitThread *)malloc(sizeof(struct argInitThread));
  if (init==NULL || newThread==NULL){
    return -1;
  }

  init ->fun = f;
  init->arg = arg;
  init->addrThreadExit = addr;
  newThread->Start(StartUserThread, init);
  currentThread->space->nbThread++;
  currentThread->space->threadCreate -> Release();
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

  //currentThread->space->execThreadSector->P();  //Le Semaphore principal qui empèche + de 4 threads d'être actifs en meme temps

  int threadSlot = currentThread->space->threadBitmap->Find();
  currentThread -> setIdMap(threadSlot);
  currentThread->space->threadBitmap->Mark(threadSlot);
  currentThread->space->threadMap[threadSlot] = currentThread;
  machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(threadSlot));
  DEBUG ('a', "Initializing thread stack register to 0x%x\n", machine->ReadRegister(StackReg));
  currentThread->space->threadCreate -> Release();
  machine->Run();
}


void do_ThreadExit(int nbProcess){

threadExit->Acquire();

  if(currentThread->space->threadBitmap!=NULL){

  currentThread->space->threadBitmap->Clear(currentThread->getIdMap());
  currentThread->space->threadMap[currentThread->getIdMap()]=NULL;
  currentThread->space->execThreadSector->V();  //on rend un jeton : la place est desormais libre
}
if(currentThread->space->nbThread == 1){
  delete currentThread->space;
  //quit();
  //printf("Exit(%d)\n",ret );
  if (nbProcess == 0) {
    interrupt->Halt();
  }
}
  currentThread->space->nbThread--;

  threadExit->Release();
  //printf("do_ThreadExit : %s\n",currentThread->getName() );
  currentThread->Finish();
}



// Libère les objets créés
void quit(){
  delete currentThread->space->execThreadSector;
  delete currentThread->space->threadBitmap;
  delete currentThread->space->threadCreate;
  delete currentThread->space;

  //delete threadExit;
//  delete threadCreate;
  printf("\n");
}


#endif
