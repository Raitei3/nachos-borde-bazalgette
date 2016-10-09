#include "syscall.h"


char s[10];
int main()
{
  GetString(s,256);
  PutString(s);
  return 0;
}
