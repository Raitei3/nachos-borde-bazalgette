
#include "syscall.h"
#include "system.h"


PageProvider::PageProvider(){
  bitmap = new BitMap(NumPhysPages);
}

int GetEmptyPage(){
  int b = bitmap->find();
  memset(PageSize*b, 0, PageSize);
  return b;
}

void RealeasePage(int i) {
  if (i <= NumPhysPages) {
    bitmap->clear(i);
  }
}

int NumAvailPages() {
  return bitmap->NumClear();
}
