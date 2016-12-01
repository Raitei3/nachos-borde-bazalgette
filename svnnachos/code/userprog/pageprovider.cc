
#include "syscall.h"
#include "system.h"


PageProvider::PageProvider(){
  bitmap = new BitMap(numPages);
}


/*int GetEmptyPage(){

}*/
