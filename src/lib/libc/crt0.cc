#include "unistd.h"
#include "stdio.h"


extern "C" {

int main(int argc, char** argv);

extern void __libc_init();

int _start(int argc, char** argv)
{
    printf("start \n");
    __libc_init();

    exit(main(argc, argv));
    return 0;
}

}