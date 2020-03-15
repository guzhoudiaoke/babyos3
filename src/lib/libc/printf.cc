#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int printf(const char* fmt, ...)
{
    const int c_buffer_size = 1024;
    char buffer[c_buffer_size] = {0};

    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    write(fd_stdout, buffer, total);
    return total;
}
