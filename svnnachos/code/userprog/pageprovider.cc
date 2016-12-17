#ifdef CHANGED
//#include "syscall.h"
#include "system.h"
#include "pageprovider.h"
#include "syscall.h"
#include "machine.h"


int x = 0;

Semaphore * pagesAllocation = new Semaphore("pagesAllocation",NumPhysPages);
BitMap * bitmap;


PageProvider::PageProvider(){
  bitmap = new BitMap(NumPhysPages);
}

int PageProvider::GetEmptyPage(){
  pagesAllocation->P();
  x++;
  //printf("P : %d\n",x );
  int b = bitmap->Find();
  memset(machine->mainMemory + b*PageSize,0,PageSize);
  //printf("%d\n",b );
  return b;
}

void PageProvider::RealeasePage(int i) {
  if (i <= NumPhysPages) {
    bitmap->Clear(i);
    pagesAllocation->V();
    x--;
    //printf("V :%d\n",x );
  }
}

unsigned int PageProvider::NumAvailPages() {
  return bitmap->NumClear();
}

#endif
