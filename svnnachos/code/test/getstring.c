#include "syscall.h"


/**
*Ce test va demander à l'utilisateur de rentrer une chaine de caractères
*dans la console. Celle-ci devra lui être retournée dans son intégralité sauf en
*cas de présence d'un '\n' d'un EOF, ou encore si elle dépasse size-1 comme pour fgets
**/

int size = 15; // le parametre size de l'appel (comme pour fgets représente le nombre limite de caractere lu)
char s[10];
int main()
{
  GetString(s,size);
  PutString(s);
  return 0;
}
