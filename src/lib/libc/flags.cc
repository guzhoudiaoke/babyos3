/*
 *	babyos/lib/libc/flags.cc
 *
 *  Copyright (C) <2020>  <Ruyi Liu>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/*
 *  2020-03-16		created
 */


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
