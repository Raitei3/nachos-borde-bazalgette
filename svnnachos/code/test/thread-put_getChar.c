#include "syscall.h"


void func(void)
{
  char c = 'a';
  PutChar(c);
  PutChar('\n');
  //ThreadExit();
  return 0;
}

int main()
{
  char c2='b';
  void * v = &(func);
  ThreadCreate(v, 0);
  PutChar(c2);
  PutChar('\n');
  //ThreadExit();
  return 0;
}
