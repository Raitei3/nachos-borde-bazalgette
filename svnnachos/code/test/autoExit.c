#include "syscall.h"

/* Vérifie que tous les threads terminent correctement sans appel à threadExit
 * et ce sur un grand nombre de threads.
 * Doit retourner 20 A */


void func();

int nbThread = 20;
char a = 'A';

int main(){
  void * v = (func);
  int i;
  for (i = 0; i < nbThread; i++) {
    ThreadCreate(v,&a);
  }
  return 0;
}

void func(void * c) {
  char* x = c;
  PutChar(*x);
}
