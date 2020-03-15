#include <string.h>
#include <unistd.h>
#include <stdio.h>

void gets(char* buf, unsigned max)
{
    memset(buf, 0, max);
    unsigned i = 0;
    while (i < max) {
        char c;
        int n = read(fd_stdin, &c, 1);
        if (n == 1) {
            if (c == '\n') {
                break;
            }
            *buf++ = c;
            i++;
        }
    }
}
