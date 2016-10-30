#include "syscall.h"


int n = 1000 ;

void func(void)
{
  char c = 'a';
  PutChar(c);
  ThreadExit();
}

int main()
{
  void * v = &(func);
  ThreadCreate(v, 0);
  while (1){}
  return 0;
}
