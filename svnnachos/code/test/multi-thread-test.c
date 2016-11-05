#include "syscall.h"

void func();
void func2();
void func3();
void func4();

char a = 'a';
char b = 'b';
char c ='c';

int main(){

  void * v = &(func);
  ThreadCreate(v,&b);
  ThreadCreate(v,&c);
  //ThreadCreate(v3,0);
  //ThreadCreate(v4,0);
  func(&a);
  return 0;
}


void func(void* c) {
  char* x = c;
  volatile int i=0;
  for (i=0; i < 15; i++)
    PutChar(*x);
  ThreadExit();
}
