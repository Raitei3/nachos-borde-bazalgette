// progtest.cc
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

#ifdef CHANGED
#include "synchconsole.h"
#include <string.h>
#endif //CHANGED
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvailHandler (void *arg)
{
    (void) arg;
    readAvail->V ();
}
static void
WriteDoneHandler (void *arg)
{
    (void) arg;
    writeDone->V ();
}

//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (const char *in, const char *out)
{
  char ch;

  readAvail = new Semaphore ("read avail", 0);
  writeDone = new Semaphore ("write done", 0);
  console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);

#ifdef CHANGED //partie 2

  for (;;)
    {
      readAvail->P ();	// wait for character to arrive
      ch = console->GetChar ();
      if (ch != '\n'){
	console -> PutChar ('<');
	writeDone->P ();
      }
      console->PutChar (ch);	// echo it!
      writeDone->P ();	// wait for write to finish
      if (ch != '\n'){
	console -> PutChar ('>');
	writeDone->P ();
    }
      if (ch == EOF || ch=='q') {
        in = "au revoir\n";
	for (unsigned i=0; i < strlen(in); i++){
	  ch = in[i];
	  console->PutChar (ch);
	  writeDone->P ();

	}
	//printf ("Nothing more, bye!\n");
	break;		// if q, quit
      }
    }

#endif //CHANGED

  delete console;
  delete readAvail;
  delete writeDone;
}


#ifdef CHANGED // partie

void
synchConsoleTest (const char * in, const char * out)
{
  printf("je me lance \n");
  char ch;

  SynchConsole* testsynchconsole = new SynchConsole(in, out);

  while ((ch = testsynchconsole->SynchGetChar()) != EOF)
  testsynchconsole->SynchPutChar(ch);

  fprintf(stderr, "EOF detected in SynchConsole!\n");

  delete testsynchconsole;
}

#endif //CHANGED
