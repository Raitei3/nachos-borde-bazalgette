#include "syscall.h"

// Teste GetString sur plusieur thread
// Doit retourner les deux chaine entré par l'utilisateur.

void funcString(void)
{
  char c[10];

  GetString(c,10);
  PutString(c);

  ThreadExit();
}

int main(){
  char c2[10];
  void * v = &(funcString);
  ThreadCreate(v, 0);

  GetString(c2,10);
  PutString(c2);

  ThreadExit();
  return 0;
}
