#include "syscall.h"


void funcString(void)
{
  char c[10];
  //while(1) {
  GetString(c,10);
  PutString(c);
  //}
  ThreadExit();
}

int main(){
  char c2[10];
  void * v = &(funcString);
  ThreadCreate(v, 0);
  //while (1) {
  GetString(c2,10);
  PutString(c2);
  //}
  ThreadExit();
//  while (1){}
  return 0;
}
