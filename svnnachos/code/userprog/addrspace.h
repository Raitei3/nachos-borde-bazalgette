// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "bitmap.h"

#ifdef CHANGED
class Semaphore;
class Lock;
class Thread;
#endif


#define UserStacksAreaSize		1024	// increase this as necessary!
#define NBTHREAD UserStacksAreaSize/256

class AddrSpace:dontcopythis
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code
    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual
    // address space


#ifdef CHANGED
    int AllocateUserStack(int x); // return new addr stack
    BitMap * threadBitmap;      //La bitmap d'allocation de la pile des threads
    void initFirstThread();
    Semaphore *execThreadSector;  //le semaphore qui permet de ne pas lancer trop de thread
    Lock *threadCreate;         // le lock qui permet de ne créer les thread que un par un
    int nbThread = 1;           // Nombre de thread sur ce processus
    Thread * threadMap[NBTHREAD]; // tableau de pointeur vers les autre thread de ce processus.
#endif



  private:

};


#endif // ADDRSPACE_H
