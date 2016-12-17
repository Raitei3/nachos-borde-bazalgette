#include "syscall.h"

int main(){
  PutString("Début de fork---------------------------------------------\n");
  ForkExec("test/putchar");
  PutString("fork vient de faire ForkExec----------------------------------------\n");
  return 1;
}
