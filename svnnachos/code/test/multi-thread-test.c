#include "syscall.h"

void func();
void func2();
void func3();
void func4();

volatile int i=0;
char c = 'a';

int main(){

  void * v = &(func);
  void * v2 = &(func2);
  void * v3 = &(func3);
  void * v4 = &(func4);
  ThreadCreate(v,0);
  ThreadCreate(v2,0);
  ThreadCreate(v3,0);
  ThreadCreate(v4,0);
  //while (1) {
  for (i; i < 3; i++)
    PutChar(c);
  //}
  //while (1){}
  ThreadExit();
  return 0;
}


void func(void) {
//while(1) {
  for (i; i < 3; i++)
    PutChar(c+1);
//}
  ThreadExit();
}

void func2(void) {
  //while(1) {
  for (i; i < 3; i++)
    PutChar(c+2);
  //}
  ThreadExit();
}

void func3(void) {
  //while(1) {
  for (i; i < 3; i++)
    PutChar(c+3);
  //}
  ThreadExit();
}

void func4(void)
{
//while(1) {
  for (i; i < 3; i++)
    PutChar(c+4);
//  }
  ThreadExit();
}
