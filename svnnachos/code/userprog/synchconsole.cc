#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *criticalPut;
static Semaphore *criticalGet;

static void ReadAvailHandler(void*arg)
{
  (void) arg; readAvail->V();
}

static void WriteDoneHandler(void*arg)
{
  (void) arg; writeDone->V();
}


SynchConsole::SynchConsole(const char *in, const char *out) {
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  criticalPut = new Semaphore("criticalGet", 1);
  console = new Console(in , out , ReadAvailHandler, WriteDoneHandler, 0);
  criticalGet = new Semaphore("criticalGet", 1);
}


  SynchConsole::~SynchConsole()
  {
    delete console;
    delete writeDone;
    delete readAvail;
  }

  void SynchConsole::SynchPutChar(int ch)
  {
    criticalPut -> P();
    console -> PutChar(ch);
    writeDone -> P();
    criticalPut -> V();
  }

  int SynchConsole::SynchGetChar()
  {
    int ch;
    criticalGet ->P();
    readAvail->P();
    ch = console -> GetChar();
    criticalGet ->V();
    return ch;
  }

  void SynchConsole::SynchPutString(const char s[])
  { //on recherche juste '\0' car on le force en amont
    while (*s != '\0') {
      console -> PutChar(*s);
      writeDone -> P();
      s++;
    }
  }

  void SynchConsole::SynchGetString(char * s, int n)
  {
    if (n > 0){
      int i=0;

      do{
        readAvail->P();
        s[i] = console->GetChar();
        i++;
      }while ((i < n-1) && (s[i-1]!='\n') && (s[i-1]!='\0') && (s[i-1] != EOF));
    }
  }


  #endif // CHANGED
