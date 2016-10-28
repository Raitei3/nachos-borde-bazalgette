#include "syscall.h"

void func(char c) {
  putchar(c);
  printf("yop");
}

int main()
{
  ThreadCreate(func, 'y');
  return 0;
}
