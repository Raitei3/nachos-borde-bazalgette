#ifdef CHANGED
//#include "syscall.h"
#include "system.h"
#include "pageprovider.h"
#include "syscall.h"
#include "machine.h"

Semaphore * pageAllocation = new Semaphore("pageAllocation",0);
BitMap * bitmap;


PageProvider::PageProvider(){
  bitmap = new BitMap(NumPhysPages);

}

int PageProvider::GetEmptyPage(){

  int b = bitmap->Find();
  memset(machine->mainMemory + b*PageSize,0,PageSize);
  return b;
}

void PageProvider::RealeasePage(int i) {
  if (i <= NumPhysPages) {
    bitmap->Clear(i);
  }
}

unsigned int PageProvider::NumAvailPages() {
  return bitmap->NumClear();
}

void PageProvider::Wait(){
  pageAllocation->P();
}

void PageProvider::Release(){
  pageAllocation->V();
}

#endif
