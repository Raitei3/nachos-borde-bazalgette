#include "syscall.h"
main()
{
  int i =0;
for (i; i<20;i++)
ForkExec("test/multi-thread-test");
return 0;
}
