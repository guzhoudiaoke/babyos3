/*
 *	babyos/lib/stdio.cc
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
 *  2020-03-07		created
 */


#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "fcntl.h"
#include "errno.h"
#include <flags.h>



__BEGIN_DECLS

static FILE __std_file[3] = {};
FILE* stdin = nullptr;
FILE* stdout = nullptr;
FILE* stderr = nullptr;



void __init_file(FILE& fp, int fd)
{
    fp.fd = fd;
    fp.errno = 0;
    fp.eof = 0;
}

void __stdio_init()
{
    stdin  = &__std_file[0];
    stdout = &__std_file[1];
    stderr = &__std_file[2];

    __init_file(*stdin,  0);
    __init_file(*stdout, 1);
    __init_file(*stderr, 2);
}

int fputc(int ch, FILE* fp)
{
    char buffer[1] = {(char) ch};
    fwrite(buffer, 1, 1, fp);
    return ch;
}

int putc(int ch, FILE* fp)
{
    return fputc(ch, fp);
}

int putchar(int ch)
{
    return putc(ch, stdout);
}

int fputs(const char* s, FILE* fp)
{
    fwrite(s, strlen(s), 1, fp);
    return 0;
}

int puts(const char* s)
{
    return fputs(s, stdout);
}

void clearerr(FILE* fp)
{
    fp->eof = false;
    fp->errno = 0;
}

int ferror(FILE* fp)
{
    return fp->errno;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* fp)
{
    if (!size) {
        return 0;
    }

    ssize_t rc = read(fp->fd, ptr, nmemb*size);
    if (rc < 0) {
        fp->errno = -rc;
        return 0;
    }

    if (rc == 0) {
        fp->eof = true;
    }

    return rc / size;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* fp)
{
    ssize_t rc = write(fp->fd, (void*) ptr, size*nmemb);
    if (rc < 0) {
        fp->errno = -rc;
        return -1;
    }
    return rc / size;
}

int fseek(FILE* stream, long offset, int whence)
{
    return 0;
}

long ftell(FILE* fp)
{
    fflush(fp);
    return lseek(fp->fd, 0ULL, SEEK_CUR);
}

int fgetpos(FILE* stream, fpos_t* pos)
{
    long rc = ftell(stream);
    if (rc == -1) {
        return -1;
    }

    *pos = rc;
    return 0;
}

int fsetpos(FILE* stream, const fpos_t* pos)
{
    return fseek(stream, (long) *pos, SEEK_SET);
}

void rewind(FILE* stream)
{
    fseek(stream, 0, SEEK_SET);
}


int fflush(FILE* stream)
{
    return 0;
}



int fgetc(FILE* fp)
{
    char ch;
    size_t nread = fread(&ch, sizeof(char), 1, fp);
    if (nread == 1) {
        return ch;
    }

    return EOF;
}

int feof(FILE* fp)
{
    return fp->eof;
}

int getc(FILE* stream)
{
    return fgetc(stream);
}


FILE* freopen(const char* pathname, const char* mode, FILE* stream)
{
    return nullptr;
}

int fclose(FILE* stream)
{
    return 0;
}


FILE* fopen(const char* pathname, const char* mode)
{
    int flags = __parse_file_flags(mode);
    int fd = open(pathname, flags);
    if (fd < 0) {
        return nullptr;
    }

    auto* fp = (FILE*) malloc(sizeof(FILE));
    __init_file(*fp, fd);

    if (flags & O_APPEND) {
        fseek(fp, 0, SEEK_END);
    }
    return fp;
}

int fprintf(FILE* fp, const char* fmt, ...)
{
    char buf[BUFSIZ] = {0};
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buf, fmt, ap);
    int ret = fwrite(buf, total, 1, fp);
    va_end(ap);
    return ret;
}

char* fgets(char* s, int size, FILE* fp)
{
    int c;
    char* ret = s;

    while ((c = fgetc(fp)) > 0) {
        *s++ = c;
        if (--size == 0) {
            return ret;
        }

        *s = '\0';
        if (c == '\n') {
            return ret;
        }
    }
    
    if (c == EOF) {
        fp->eof = 1;
        return ret == s ? nullptr : ret;
    }

    return nullptr;
}

int ungetc(int c, FILE* fp)
{
    return 0;
}

int setvbuf(FILE* fp, char* buf, int mode, size_t size)
{
    return 0;
}

__END_DECLS