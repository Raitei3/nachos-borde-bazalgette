// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "../threads/scheduler.h"

#ifdef CHANGED

#include "userthread.h"

static Lock *spaceCreate = new Lock("spaceCreate");
void fork(void* arg);
int copyStringFromMachine(int from, char *to, unsigned size);
int copyStringToMachine(int from, char *to, unsigned size);
void releaseMutex(Thread* thread);

int nbThreadNoyau = 1;
int nbProcess = 1;
int put = 0;


#endif //CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
  int pc = machine->ReadRegister (PCReg);
  machine->WriteRegister (PrevPCReg, pc);
  pc = machine->ReadRegister (NextPCReg);
  machine->WriteRegister (PCReg, pc);
  pc += 4;
  machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
  int type = machine->ReadRegister (2);

  switch (which)
  {
    case SyscallException:
    {
      switch (type)
      {
        case SC_Halt:
        {
          DEBUG ('s', "Shutdown, initiated by user program.\n");
          interrupt->Halt ();
          break;
        }

        #ifdef CHANGED

        case SC_Exit:
        {

          DEBUG ('s', "Shutdown, initiated auto\n");
          IntStatus oldLevel = interrupt->SetLevel (IntOff);// on bloque les interruptions
          int ret = machine -> ReadRegister(4);

          if (currentThread->space->threadBitmap!=NULL){
            int slot = currentThread->getIdMap();

            for(int i =0;i<NBTHREAD;i++){

              if(currentThread->space->threadMap[i]!=NULL && i != slot){
                releaseMutex(currentThread->space->threadMap[i]);
                currentThread->space->threadMap[i]->setStatus(A_DETRUIRE);// on met le status sur a_detruire désormais interprété par l'ordonnanceur
              }
            }
            nbProcess--;
          if (nbProcess == 1) {
            printf("Exit(%d)\n",ret );
            interrupt->Halt();
          }
          else{
            printf("Exit(%d)\n",ret );
            quit();
            (void) interrupt->SetLevel (oldLevel);
            currentThread->Finish();
          }
        }
        else{
          printf("Exit(%d)\n",ret );
          quit();
          printf("la:%s\n",currentThread->getName() );
          if(nbProcess==1)

            interrupt->Halt();
        }
          break;
        }

        case SC_PutChar:
        {
          DEBUG ('s', "call PutChar.\n");
          synchconsole -> SynchPutChar(machine->ReadRegister(4)); //on récupère simplement le parametre de l'appel système
                                                                  //et on l'envoie à notre synchconsole.
          break;
        }

        case SC_GetChar:
        {
          DEBUG ('s', "call GetChar.\n");
          char s;
          s = synchconsole -> SynchGetChar(); //on récupère le caractère grâce à la synchconsole
          machine -> WriteRegister(2,s);      //et on l'écrit dans le registre 2, valeur de retour de l'appel système.
          break;
        }

        case SC_PutString:
        {
          DEBUG ('s', "call PutString.\n");
          int from = machine -> ReadRegister(4);     //on recupère l'adresse de la chaine
          char to[MAX_STRING_SIZE];                  //on créer notre tampon
          while (copyStringFromMachine(from, to , MAX_STRING_SIZE) == MAX_STRING_SIZE )    //on remplit le tampon
          {                                                   //on recommence tant qu'on utilise tout le buffer
          synchconsole -> SynchPutString(to);                         //et on envoie le tampon à la console.
          from += MAX_STRING_SIZE-1;
        }
        synchconsole -> SynchPutString(to);
        break;
      }

      /** Comme pour fgets on prendra soin de s'arreter lorsqu'on rencontre \n et de n'écrire que
      *   size-1 caractères de la chaine.
      **/

      case SC_GetString:
      {
        DEBUG ('s', "call GetString.\n");
        char s[MAX_STRING_SIZE];
        int size = machine -> ReadRegister(5);  //on recupère les paramètres de l'appel
        int from = machine -> ReadRegister(4);
        int i=0 , j=MAX_STRING_SIZE-1;
        while(size / MAX_STRING_SIZE > 0 && j == MAX_STRING_SIZE-1)   //on compare la taille du buffer avec celle
        {                                                             //de size pour savoir quand on doit s'arrêter
        synchconsole -> SynchGetString(s, MAX_STRING_SIZE);         //ou le nombre de cractere retourné pour savoir si la chaine
        //s'est finie plus tôt.
        j=copyStringToMachine(from+i,s,MAX_STRING_SIZE); //on recopie le contenu du buffer à l'adresse du paramètre
        size -= j;                                                  //de l'appel système
        i += j;
      }
      if(j == MAX_STRING_SIZE-1)
      {
        synchconsole -> SynchGetString(s, size);
        j=copyStringToMachine(from+i,s,size);
      }
      break;
    }

    case SC_PutInt:
    {
      DEBUG ('s', "call GetInt.\n");
      char s[MAX_STRING_SIZE];
      int from = machine -> ReadRegister(4);
      snprintf(s,MAX_STRING_SIZE,"%d",from);
      printf("%s\n",s);
      break;
    }

    case SC_GetInt:
{
    int x = 0;
    int n=machine->ReadRegister(4);
    char s[MAX_STRING_SIZE];
    synchconsole -> SynchGetString(s, MAX_STRING_SIZE);
    sscanf(s,"%d",&x);
    machine->WriteMem(n,4,x);
    break;
  }


    case SC_ThreadCreate:
    {
      DEBUG ('s', "call ThreadCreate.\n");
      int thread;
      int addrThreadExit = machine-> ReadRegister(6);  //on recupère l'adresse de ThreadExit

      thread = do_ThreadCreate(machine -> ReadRegister(4),machine -> ReadRegister(5),addrThreadExit);
      machine -> WriteRegister(2,thread);  // on écrit la valeur de retour de ThreadCreate
      break;
    }

    case SC_ThreadExit:
    {
      DEBUG ('s', "call ThreadExit.\n");
      if(currentThread->space->nbThread == 1){
        nbProcess--;
      }
      do_ThreadExit(nbProcess);
      break;
    }



    case SC_ForkExec:
    {

      DEBUG ('s', "call ForkExec.\n");
      int from = machine -> ReadRegister(4);
      char to[MAX_STRING_SIZE];
      copyStringFromMachine(from, to , MAX_STRING_SIZE);
      const char *to2 = to;
      OpenFile *executable = fileSystem->Open (to2);
      Thread *thread;

      if (executable == NULL)
        {
  	  printf ("Unable to open file %s\n", to);
  	  return;
        }
      nbThreadNoyau++;
      /** les 2 lignes suivantes permettent de donner des noms différents mais on n'arrive pas à éviter les fuites memoire*/
      //char * s =(char*) malloc(sizeof(char)*50);
      //sprintf(s,"thread-noyau-%d",nbThreadNoyau);

      char s[50] = "thread-noyau";

      thread = new Thread(s);
      thread->Start(fork,executable);
      nbProcess++;
      break;
    }

  #endif //CHANGED





  default:
  {
    printf("Unimplemented system call %d\n", type);
    ASSERT(FALSE);
  }
}

// Do not forget to increment the pc before returning!
UpdatePC ();
break;
}




case PageFaultException:
if (!type) {
  printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
  ASSERT (FALSE);
} else {
  printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
  ASSERT (FALSE);	// For now
}

default:
printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
ASSERT (FALSE);
}
}

#ifdef CHANGED

/**
  *Fonction qui initialise la nouvelle addrspace du nouveau processus
  */

void fork(void* arg){
      AddrSpace *space;
      spaceCreate->Acquire();
      space = new AddrSpace ((OpenFile*)arg);
      spaceCreate->Release();
      currentThread->space = space;
      space->InitRegisters();
      space->RestoreState();
      delete (OpenFile*)arg;
      machine->Run();
}

int copyStringFromMachine(int from, char *to, unsigned size){

  int value=0;
  unsigned i=0;
  do{
    machine -> ReadMem(from,1,&value);  //on recopie caractère par caractère dans value
    char s = (char)value;								// on caste (chose possible avec un int en char)
    to[i]=s;
    from++;												      //on incrémente notre adresse virtuelle
    i++;
  }while ((i < size-1) && (to[i-1] != '\0'));

  to[i] = '\0';											    //on force toujours l'écriture d'un '\0'
  i++;
  return i;
}


int copyStringToMachine(int from, char *to, unsigned size)
{
  int value = 0;
  unsigned i=0;
  do
  {
    value = to[i];
    machine -> WriteMem(from,1,value);  //on fait l'inverse de ReadMem
    i++;
    from++;
  } while (i < size-1 && to[i-1]!='\0' && to[i-1]!='\n');

  if (to[i-1]=='\n'){
    machine -> WriteMem(from,1,'\0');				//comme fgets on force l'écriture d'un '\0'
    i++;
  }
  return i;
}

/**
  *Fonction qui se charge de parcourir la liste de threads qui ont pris des mutexs et les libère
  */

void releaseMutex(Thread* thread){
  while (!(thread->listSem->IsEmpty())) {
    Semaphore * s = (Semaphore *)thread->listSem->Remove();

    if(strcmp(s->getName(),"write done")==0 || strcmp(s->getName(),"read avail")==0 ){
    }
    else{
      s->V();
    }
  }
  while (!(thread->listLock->IsEmpty())) {
    Lock * l = (Lock *)thread->listLock->Remove();
    l->Release();
  }
}

#endif // CHANGED
