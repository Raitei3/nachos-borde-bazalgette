#include "syscall.h"


int n = 1000 ;

void func(void)
{
  char c = 'a';
  PutChar(c);
}

int main()
{
  void * v = &(func);
  ThreadCreate(v, 0);
  while (n>0){
    n=n-1;
  }
  return 0;
}
