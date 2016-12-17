#include "syscall.h"

/*void print(char c, int n) {
  int i;
  for (i = 0; i < n; i++) {
    PutChar(c + i);
  }
  PutChar('\n');
}*/
int main() {
  PutString("début de PutChar-------------------------------------------------\n");
  //print('a',4);
  PutChar('A');
  return 1;
}
