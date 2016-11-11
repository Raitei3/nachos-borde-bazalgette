#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Lock *criticalPutChar;
static Lock *criticalGetChar;
static Lock *criticalGetString;
static Lock *criticalPutString;

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
  criticalPutChar = new Lock("criticalPutChar");
  criticalGetChar = new Lock("criticalGetChar");
  criticalGetString = new Lock("criticalGetString");
  criticalPutString = new Lock("criticalPutString");
  console = new Console(in , out , ReadAvailHandler, WriteDoneHandler, 0);
}


SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
  delete criticalPutChar;
  delete criticalGetString;
  delete criticalGetChar;
  delete criticalPutString;
}

void SynchConsole::SynchPutChar(int ch)
{
  criticalPutChar -> Acquire();
  console -> PutChar(ch);
  writeDone -> P();
  criticalPutChar -> Release();
}

int SynchConsole::SynchGetChar()
{
  int ch;
  criticalGetChar ->Acquire();
  readAvail->P();
  ch = console -> GetChar();
  criticalGetChar ->Release();
  return ch;
}

void SynchConsole::SynchPutString(const char s[])
{ //on recherche juste '\0' car on le force en amont
criticalPutString->Acquire();
while (*s != '\0') {
  SynchPutChar(*s);
  s++;
}
criticalPutString->Release();
}

void SynchConsole::SynchGetString(char * s, int n)
{
  criticalGetString->Acquire();
  if (n > 0){
    int i=0;

    do{
      s[i] = SynchGetChar();
      i++;
    }while ((i < n-1) && (s[i-1]!='\n') && (s[i-1]!='\0') && (s[i-1] != EOF));  // EOF valeur retournée par getchar mais on va pas trouver ca dans un char *. a supprimer
  }
  criticalGetString->Release();
}


#endif // CHANGED
