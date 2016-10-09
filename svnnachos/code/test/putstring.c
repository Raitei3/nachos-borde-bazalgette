#include "syscall.h"


/** test de l'appel systeme PutString qui doit écrire dans la console
*   les chaine suivante a l'identique
**/

char * s = "test\n";
char * s2 = "test d'une chaine plus longue que MAX_STRING_SIZE.\n";
char * s3 = "test\n\0go";

int main()
{

  PutString(s);
  PutString(s2);
  PutString(s3);
  return 0;
}
