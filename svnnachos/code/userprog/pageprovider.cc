#ifdef CHANGED
//#include "syscall.h"
#include "system.h"
#include "pageprovider.h"

BitMap * bitmap;


PageProvider::PageProvider(){
  bitmap = new BitMap(NumPhysPages);
}

int PageProvider::GetEmptyPage(){
  int b = bitmap->Find();
  for (int i = 0 ; i<PageSize ; i++)
  machine->WriteMem(b,1,0);
  return b;
}

void PageProvider::RealeasePage(int i) {
  if (i <= NumPhysPages) {
    bitmap->Clear(i);
  }
}

int PageProvider::NumAvailPages() {
  return bitmap->NumClear();
}

#endif
