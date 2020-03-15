#include <fcntl.h>
#include <errno.h>

int __parse_file_flags(const char* mode)
{
    int m = 0, o = 0;

    switch (*mode++) {
    case 'r':
        m = O_RDONLY;
        o = 0;
        break;
    case 'w':
        m = O_WRONLY;
        o = O_CREAT | O_TRUNC;
        break;
    case 'a':
        m = O_WRONLY;
        o = O_CREAT | O_APPEND;
        break;
    default:
        errno = EINVAL;
        return 0;
    }

    while (*mode != '\0') {
        switch (*mode++) {
        case 'b':
            break;
        case '+':
            m = O_RDWR;
        default:
            break;
        }
    }

    return m | o;
}
