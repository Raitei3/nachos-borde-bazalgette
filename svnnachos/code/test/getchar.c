#include "syscall.h"

int main()
{
  char s;
  s = GetChar();
  PutChar(s);
  return 0;
}
