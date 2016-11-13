#include "syscall.h"

char c = 'a';

void func() {
  PutChar(c);
  ThreadExit();
}

int main() {
  void * p = &(func);
  ThreadCreate(p, 0);
  PutChar(c+1);
  return 0;
}
