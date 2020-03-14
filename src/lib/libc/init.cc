#include "stdlib.h"
#include "stdio.h"

extern "C" {

void __libc_init()
{
    malloc_init();
    __stdio_init();
}


}