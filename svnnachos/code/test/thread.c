#include "syscall.h"

// test PutChar sur 2 thread
// doit retourner a et b 

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
