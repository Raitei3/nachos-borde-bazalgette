#include "syscall.h"

/** Test de l'appel système GetChar. A l'éxécution, demande d'entrer un
*caractère dans la console et doit le retourner.
**/


int main()
{
  char s;
  s = GetChar();
  PutChar(s);
  return 0;
}
