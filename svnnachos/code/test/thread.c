#include "syscall.h"


int n = 1000 ;

void func(void)
{
  char c = 'a',d;
  while(1) {
  c=GetChar();
  PutChar(c);
  }
  ThreadExit();
}

int main()
{
  char c2='b';
  void * v = &(func);
  ThreadCreate(v, 0);
  while (1) {
  c2 = GetChar();
  PutChar(c2+1);
  }
  while (1){}
  return 0;
}
