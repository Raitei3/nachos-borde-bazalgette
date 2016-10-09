#include "syscall.h"

/** Test de l'appel systeme GetChar. a l'éxécution demande d'entré un
*caractere dans la console et doit le retourner .
**/


int main()
{
  char s;
  s = GetChar();
  PutChar(s);
  return 0;
}
