#include "syscall.h"

void func();

int nbThread = 30;
char a = 'A';


int main(){
  void * v = (func);
  int i;
  for (i = 0; i < nbThread; i++) {
    ThreadCreate(v,&a);
  }
  return 0;
}


void func(void* c) {
  char* x = c;
  volatile int i=0;
  for (i=0; i < 3; i++)
    PutChar(*x+i);

}
