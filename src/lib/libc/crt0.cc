#include "unistd.h"


extern "C" {

int main(int argc, char** argv);

extern void __libc_init();

int _start(int argc, char** argv)
{
    __libc_init();

    exit(main(argc, argv));
    return 0;
}

}