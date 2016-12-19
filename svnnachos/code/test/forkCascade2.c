#include "syscall.h"
int main()
{
  int i = 0;
  for (i; i<10; i++) {
    ForkExec("test/multi-thread-test");
  }
  return 0;
}
