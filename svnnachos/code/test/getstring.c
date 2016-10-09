#include "syscall.h"


char s[10];
int main()
{
  GetString(s,30);
  PutString(s);
  return 0;
}
