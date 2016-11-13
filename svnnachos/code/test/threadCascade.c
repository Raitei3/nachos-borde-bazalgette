#include "syscall.h"


void func2(void)
{
  char c = 'a';
  PutChar(c);
  PutChar('\n');
  return 0;
}

void func(void){
  void * w = &(func2);
  ThreadCreate(w,0);
}

int main()
{
  char c2='b';
  void * v = &(func);
  ThreadCreate(v, 0);
  PutChar(c2);
  PutChar('\n');
  return 0;
}
