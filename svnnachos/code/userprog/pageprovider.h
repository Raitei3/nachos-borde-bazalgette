#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"
//#include "syscall.h"
#include "system.h"

class PageProvider:dontcopythis{
  public:

    PageProvider();
    ~PageProvider();
    int GetEmptyPage();
    void RealeasePage(int i);
  unsigned int NumAvailPages();

  private:

    BitMap * bitmap;

};
#endif
#endif
