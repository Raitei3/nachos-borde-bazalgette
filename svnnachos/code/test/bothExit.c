#include "syscall.h"

char *c = "test\n";

void func() {
  PutString(c);
  ThreadExit();
}

int main() {
  void * p = &(func);
  ThreadCreate(p, 0);
  PutString(c);
  ThreadExit();
  return 0;
}
