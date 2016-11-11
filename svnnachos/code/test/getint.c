#include "syscall.h"

int main()
{
  int n[1];
  GetInt(n);
  PutInt(n[0]);
  return 0;
}
