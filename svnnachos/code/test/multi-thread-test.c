#include "syscall.h"

//Test l'éxécution d'un grand nombre de thread
// doit retourner 30 * ABC soit 90 lettre
//(l'ordre est aléatoire et dépend de l'ordonanceur).


void func();

int nbThread = 30;
char a = 'A';


int main(){
  void * v = (func);
  int i;
  for (i = 0; i < nbThread; i++) {
    ThreadCreate(v,&a);
  }
  ThreadExit();
  return 0;
}


void func(void* c) {
  char* x = c;
  volatile int i=0;
  for (i=0; i < 3; i++)
    PutChar(*x+i);
  ThreadExit();
}
