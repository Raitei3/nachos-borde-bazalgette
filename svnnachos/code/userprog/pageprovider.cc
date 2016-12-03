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
  /*int * p = (int*) b+1;
  void * p2 = (void*)p;
  printf("%p\n",p2 );
  memset(p2,0,PageSize);*/
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

#endif
