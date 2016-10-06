#include "syscall.h"

int main()
{
  char s = GetChar();
  PutChar(s);
  return 0;
}
