#include "syscall.h"



int main()
{
  char* s = GetString();
  PutString(s);
  return 0;
}
