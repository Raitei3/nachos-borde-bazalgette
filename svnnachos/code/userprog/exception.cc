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
        case SC_PutChar:
        {
          DEBUG ('s', "call PutChar.\n");
          synchconsole -> SynchPutChar(machine->ReadRegister(4));
          break;
        }

        case SC_PutString:
        {
          DEBUG ('s', "call PutString.\n");
          int from = machine -> ReadRegister(4);
          char to[MAX_STRING_SIZE];
          while (machine -> copyStringFromMachine(from, to , MAX_STRING_SIZE-1) == MAX_STRING_SIZE-1)
          {
            synchconsole -> SynchPutString(to);
            from += MAX_STRING_SIZE-1;

          }
          synchconsole -> SynchPutString(to);
          break;

        }
        case SC_Exit:
        {
          DEBUG ('s', "Shutdown, initiated auto\n");
          int ret = machine -> ReadRegister(4);
          printf("Exit(%d)\n",ret);
          interrupt->Halt ();
          break;
        }

        case SC_GetChar:
        {
          DEBUG ('s', "call GetChar.\n");
          char s;
          s = synchconsole -> SynchGetChar();
          machine -> WriteRegister(2,s);
          break;
        }


        case SC_GetString:
        {
          DEBUG ('s', "call GetString.\n");
          char s[MAX_STRING_SIZE];
          int size = machine -> ReadRegister(5);
          int from = machine -> ReadRegister(4);
          int i=0 , j=MAX_STRING_SIZE-1;
          int x=0;
          while(size / MAX_STRING_SIZE > 0 && j == MAX_STRING_SIZE-1){
            synchconsole -> SynchGetString(s, MAX_STRING_SIZE);
            j=machine -> copyStringToMachine(from+i,s,MAX_STRING_SIZE);
            size -= j;
            i += j;
          }
          if(j == MAX_STRING_SIZE-1){
            synchconsole -> SynchGetString(s, size);
            j=machine -> copyStringToMachine(from+i,s,size);
          }
          break;
        }




        /*case SC_GetInt:
        {
        DEBUG ('s', "call GetInt.\n");
        char s[MAX_STRING_SIZE];
        int value;
        int from = machine -> ReadRegister(4);
        machine -> ReadMem(from,4,&value);
        snprintf(s,MAX_STRING_SIZE,"%d",value);
        machine -> copyStringToMachine(from,s,MAX_STRING_SIZE);
        printf("%s\n",s);
        break;
        }

        case SC_PutInt:
        {
        int n;
        char s[5];
        synchconsole -> SynchGetString(s, 5);
        sscanf(s,"%d",&n);
        machine -> WriteRegister(4,n);
        printf("%d\n",machine -> ReadRegister(4));
        break;
      }
        */

        /*  case SC_PutInt:
        {
        int[]
        int n = machine -> ReadRegister(4);
        char s[MAX_STRING_SIZE];
        snprintf(s,"%d",n);

        break;
      }
        */

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
