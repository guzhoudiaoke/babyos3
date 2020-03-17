/*
 *	babyos/lib/libc/fileop.cc
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
 *  2020-03-15		created
 */



#include <sys/cdefs.h>
#include "stddef.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


__BEGIN_DECLS


pid_t getpid(void) {
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}

char* tmpnam(char * s)
{
    printf("%s not implemented\n", __FUNCTION__);
    return NULL;
}


int remove(const char* pathname)
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}

int rename(const char* oldpath, const char* newpath)
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}


FILE* tmpfile()
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return NULL;
}


__END_DECLS