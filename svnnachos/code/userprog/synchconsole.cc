#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *criticalPutChar;
static Semaphore *criticalGetChar;
static Semaphore *criticalGetString;
static Semaphore *criticalPutString;

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
  criticalPutChar = new Semaphore("criticalPutChar", 1);
  console = new Console(in , out , ReadAvailHandler, WriteDoneHandler, 0);
  criticalGetChar = new Semaphore("criticalGetChar", 1);
  criticalGetString = new Semaphore("criticalGetString",1);
  criticalPutString = new Semaphore("criticalPutString",1);

}


  SynchConsole::~SynchConsole()
  {
    delete console;
    delete writeDone;
    delete readAvail;
  }

  void SynchConsole::SynchPutChar(int ch)
  {
    criticalPutChar -> P();
    console -> PutChar(ch);
    writeDone -> P();
    criticalPutChar -> V();
  }

  int SynchConsole::SynchGetChar()
  {
    int ch;
    criticalGetChar ->P();
    readAvail->P();
    ch = console -> GetChar();
    criticalGetChar ->V();
    return ch;
  }

  void SynchConsole::SynchPutString(const char s[])
  { //on recherche juste '\0' car on le force en amont
  criticalPutString->P();
    while (*s != '\0') {
      SynchPutChar(*s);
      s++;
    }
    criticalPutString->V();
  }

  void SynchConsole::SynchGetString(char * s, int n)
  {
    criticalGetString->P();
    if (n > 0){
      int i=0;

      do{
        s[i] = SynchGetChar();
        i++;
      }while ((i < n-1) && (s[i-1]!='\n') && (s[i-1]!='\0') && (s[i-1] != EOF));
    }
    criticalGetString->V();
  }


  #endif // CHANGED
