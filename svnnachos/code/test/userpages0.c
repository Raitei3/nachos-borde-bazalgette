#include "syscall.h"

char c = 'A';

int main() {
  int i;
  for (i=0; i<10; i++) {
    PutChar(c+i);
  }
  return 0;
}
