#include "syscall.h"

/* Vérifie qu'il n'y a pas d'erreur a l'apel de threadExit pour le userThread et le main.
  * Doit retourner 2* test et terminer sans erreur.*/

char *c = "test\n";

void func() {
  PutString(c);
  ThreadExit();
}

int main() {
  void * p = &(func);
  ThreadCreate(p, 0);
  PutString(c);
  ThreadExit();
  return 0;
}
