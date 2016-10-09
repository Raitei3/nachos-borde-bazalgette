#include "syscall.h"
/*#include "../machine/machine.h"
#include "../userprog/synchconsole.h"
#include "../userprog/progtest.h"
#include "../threads/utility.h"
#include "../threads/system.h"*/

char * s = "test\n";
char * s2 = "test d'une chaine trop longue qui devrait planter si je me suis gouré mais c'est peu probable vu que je suis une machine parfaite créer pour detruire l'humanité.\n";
char * s3 = "test\n\0go";

int main()
{

  PutString(s);
  PutString(s2);
  PutString(s3);
  return 0;
}
