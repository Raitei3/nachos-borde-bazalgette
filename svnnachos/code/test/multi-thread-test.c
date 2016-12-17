#include "syscall.h"

/*Teste l'exécution d'un grand nombre de threads.
* doit retourner 30 * ABC soit 90 lettres
(l'ordre est aléatoire et dépend de l'ordonnanceur). */


void func();

int nbThread = 30;

char a = 'A';


int main(){
  void * v = (func);
  int i;
  for (i = 0; i < nbThread; i++) {
    ThreadCreate(v,&a);
  }
//  ThreadExit();
  return 0;
}


void func(void* c) {
  char* x = c;
  volatile int i=0;
  for (i=0; i < 3; i++)
    PutChar(*x+i);
    return 1;
}
