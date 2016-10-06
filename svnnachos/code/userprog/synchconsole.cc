#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;


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
   console = new Console(in , out , ReadAvailHandler, WriteDoneHandler, 0);}


SynchConsole::~SynchConsole()
{
   delete console;
   delete writeDone;
   delete readAvail;
}
void SynchConsole::SynchPutChar(int ch)
{
   console -> PutChar(ch);
   writeDone -> P();
}

int SynchConsole::SynchGetChar()
{
   int ch;
   readAvail->P();
   return ch = console -> GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
   while (*s != '\0') {
      console -> PutChar(*s);
      writeDone -> P();
      s++;
   }
}

void SynchConsole::SynchGetString(char * s, int n)
{
  for (int i=0; i<n; i++) {
    readAvail->P();
    s[i] = console->GetChar();
  }
}


#endif // CHANGED
