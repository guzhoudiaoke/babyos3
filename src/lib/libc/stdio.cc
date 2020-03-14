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


#define CHARACTER(ch)       (ch & 0xff)

extern "C" {

static FILE __default_streams[3] = {};
FILE* stdin = nullptr;
FILE* stdout = nullptr;
FILE* stderr = nullptr;



void init_FILE(FILE& fp, int fd, int mode)
{
    fp.fd = fd;
    fp.buffer = fp.default_buffer;
    fp.buffer_size = BUFSIZ;
    fp.mode = mode;
}

static FILE* make_FILE(int fd)
{
    auto* fp = (FILE*)malloc(sizeof(FILE));
    memset(fp, 0, sizeof(FILE));
    init_FILE(*fp, fd, 0);
    return fp;
}

void __stdio_init()
{
    stdin = &__default_streams[0];
    stdout = &__default_streams[1];
    stderr = &__default_streams[2];
    init_FILE(*stdin, 0,  _IOFBF);
    init_FILE(*stdout, 1, _IOFBF);
    init_FILE(*stderr, 2, _IONBF);
}


int sprint_int(char* buffer, int n, int width, int base, bool sign)
{
    static char digits[] = "0123456789abcdef";
    char buf[16] = {0};

    unsigned num = (unsigned)n;
    if (sign && (sign = (n < 0))) {
        num = -n;
    }

    int i = 0;
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num != 0);

    if (sign) {
        buf[i++] = '-';
    }

    if (width < i) {
        width = i;
    }

    int len = i;
    while (--i >= 0) {
        *buffer++ = buf[i];
    }

    for (int i = 0; i < width - len; i++) {
        *buffer++ = ' ';
    }

    return width;
}

int sprint_str(char* buffer, char* s, int width)
{
    if (s == nullptr) {
        return 0;
    }

    int len = strlen(s);
    if (width < len) {
        width = len;
    }

    strcat(buffer, s);
    buffer += len;
    for (int i = 0; i < width - len; i++) {
        *buffer++ = ' ';
    }
    return width;
}

bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int sprint_long(char* buffer, long n, int width, int base, bool sign)
{
    if (base <= 0) {
        return -1;
    }

    const static char digits[] = "0123456789abcdef";
    char buf[16] = {0};

    unsigned long num = (unsigned long)n;
    if (sign && (sign = (n < 0))) {
        num = -n;
    }

    int i = 0;
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num != 0);

    if (sign) {
        buf[i++] = '-';
    }

    if (width < i) {
        width = i;
    }

    for (int j = 0; j < width - i; j++) {
        *buffer++ = base == 16 ? '0' : ' ';
    }
    while (--i >= 0) {
        *buffer++ = buf[i];
    }

    return width;
}
int vsprintf(char *buffer, const char* fmt, va_list ap)
{
    buffer[0] = '\0';
    if (fmt == nullptr) {
        return 0;
    }

    char c;
    int total = 0, width = 0;
    bool prefix_l = false, sign = false;

    for (int i = 0; (c = CHARACTER(fmt[i])) != 0; i++) {
        if (c != '%') {
            buffer[total++] = c;
            continue;
        }

        c = CHARACTER(fmt[++i]);
        if (c == '\0') {
            break;
        }

        width = 0;
        while (c != '\0' && is_digit(c)) {
            width = width * 10 + c - '0';
            c = CHARACTER(fmt[++i]);
        }

        if (c == '\0') {
            break;
        }

        if (c == 'l') {
            prefix_l = true;
            c = CHARACTER(fmt[++i]);
            if (c == 'l') {
                prefix_l = true;
                c = CHARACTER(fmt[++i]);
            }
        }

        if (c == '\0') {
            break;
        }

        switch (c) {
        case 'd':
        case 'u':
            sign = c == 'd';
            if (prefix_l) {
                total += sprint_long(buffer + total, va_arg(ap, unsigned long), width, 10, sign);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int), width, 10, sign);
            }
            break;
        case 'x':
        case 'p':
            if (c == 'p') {
                prefix_l = true;
                buffer[total++] = '0';
                buffer[total++] = 'x';
            }
            if (prefix_l) {
                total += sprint_long(buffer + total, va_arg(ap, unsigned long), width, 16, false);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int), width, 16, false);
            }
            break;
        case 'c':
            buffer[total++] = (char) CHARACTER(va_arg(ap, int));
            break;
        case 's':
            total += sprint_str(buffer + total, va_arg(ap, char *), width);
            break;
        case '%':
            buffer[total++] = '%';
            break;
        default:
            buffer[total++] = '%';
            buffer[total++] = c;
            break;
        }

        if (prefix_l) {
            prefix_l = false;
        }
    }

    return total;
}


int sprintf(char* buffer, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    return total;
}

/* only support %d %u %x %p %c %s */
int printf(const char *fmt, ...)
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

//void puts(char* buf)
//{
//    write(fd_stdout, buf, strlen(buf));
//}
//

int snprintf(char * buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buf, fmt, ap);
    va_end(ap);

    return total;
}




int fputc(int ch, FILE* stream)
{
    assert(stream);
    assert(stream->buffer_index < stream->buffer_size);
    stream->buffer[stream->buffer_index++] = ch;
    if (stream->buffer_index >= stream->buffer_size)
        fflush(stream);
    else if (stream->mode == _IONBF || (stream->mode == _IOLBF && ch == '\n'))
        fflush(stream);
    if (stream->eof || stream->error)
        return EOF;
    return (uint8_t)ch;
}

int putc(int ch, FILE* stream)
{
    return fputc(ch, stream);
}

int putchar(int ch)
{
    return putc(ch, stdout);
}

int fputs(const char* s, FILE* stream)
{
    for (; *s; ++s) {
        int rc = putc(*s, stream);
        if (rc == EOF)
            return EOF;
    }
    return 1;
}

int puts(const char* s)
{
    int rc = fputs(s, stdout);
    if (rc == EOF)
        return EOF;
    return fputc('\n', stdout);
}

void clearerr(FILE* stream)
{
    assert(stream);
    stream->eof = false;
    stream->error = 0;
}

int ferror(FILE* stream)
{
    return stream->error;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    assert(stream);
    if (!size)
        return 0;

    ssize_t nread = 0;

    if (stream->have_ungotten) {
        // FIXME: Support ungotten character even if size != 1.
        ASSERT(size == 1);
        ((char*)ptr)[0] = stream->ungotten;
        stream->have_ungotten = false;
        --nmemb;
        if (!nmemb)
            return 1;
        ptr = &((char*)ptr)[1];
        ++nread;
    }

    ssize_t rc = read(stream->fd, ptr, nmemb * size);
    if (rc < 0) {
        stream->error = errno;
        return 0;
    }
    if (rc == 0)
        stream->eof = true;
    nread += rc;
    return nread / size;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    assert(stream);
    auto* bytes = (const uint8_t*)ptr;
    ssize_t nwritten = 0;
    for (size_t i = 0; i < (size * nmemb); ++i) {
        int rc = fputc(bytes[i], stream);
        if (rc == EOF)
            break;
        ++nwritten;
    }
    return nwritten / size;
}

int fseek(FILE* stream, long offset, int whence)
{
    assert(stream);
    fflush(stream);
    off_t off = lseek(stream->fd, offset, whence);
    if (off < 0)
        return off;
    stream->eof = false;
    stream->error = 0;
    stream->have_ungotten = false;
    stream->ungotten = 0;
    return 0;
}

long ftell(FILE* stream)
{
    assert(stream);
    fflush(stream);
    return lseek(stream->fd, 0, SEEK_CUR);
}

int fgetpos(FILE* stream, fpos_t* pos)
{
    assert(stream);
    assert(pos);

    long val = ftell(stream);
    if (val == -1L)
        return 1;

    *pos = val;
    return 0;
}

int fsetpos(FILE* stream, const fpos_t* pos)
{
    assert(stream);
    assert(pos);
    return fseek(stream, (long) *pos, SEEK_SET);
}

void rewind(FILE* stream)
{
    ASSERT(stream);
    fseek(stream, 0, SEEK_SET);
    ASSERT(rc == 0);
}


int fflush(FILE* stream)
{
    if (!stream) {
        return 0;
    }
    if (!stream->buffer_index)
        return 0;
    int rc = write(stream->fd, stream->buffer, stream->buffer_index);
    stream->buffer_index = 0;
    stream->error = 0;
    stream->eof = 0;
    stream->have_ungotten = false;
    stream->ungotten = 0;
    if (rc < 0) {
        stream->error = errno;
        return EOF;
    }
    return 0;
}



int fgetc(FILE* stream)
{
    assert(stream);
    char ch;
    size_t nread = fread(&ch, sizeof(char), 1, stream);
    if (nread == 1)
        return ch;
    return EOF;
}

int feof(FILE* stream)
{
    assert(stream);
    return stream->eof;
}

int getc(FILE* stream)
{
    return fgetc(stream);
}


FILE* freopen(const char* pathname, const char* mode, FILE* stream)
{
    (void)pathname;
    (void)mode;
    (void)stream;
    ASSERT_NOT_REACHED();
    return nullptr;
}

int fclose(FILE* stream)
{
    fflush(stream);
    int rc = close(stream->fd);
    if (stream != &__default_streams[0] && stream != &__default_streams[1] && stream != &__default_streams[2])
        free(stream);
    return rc;
}


FILE* fopen(const char* pathname, const char* mode)
{
    int flags = 0;
    // NOTE: rt is a non-standard mode which opens a file for read, explicitly
    // specifying that it's a text file
    if (!strcmp(mode, "r") || !strcmp(mode, "rb") || !strcmp(mode, "rt"))
        flags = O_RDONLY;
    else if (!strcmp(mode, "r+") || !strcmp(mode, "rb+"))
        flags = O_RDWR;
    else if (!strcmp(mode, "w") || !strcmp(mode, "wb"))
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (!strcmp(mode, "w+") || !strcmp(mode, "wb+"))
        flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (!strcmp(mode, "a") || !strcmp(mode, "ab"))
        flags = O_WRONLY | O_APPEND | O_CREAT;
    else if (!strcmp(mode, "a+") || !strcmp(mode, "ab+"))
        flags = O_RDWR | O_APPEND | O_CREAT;
    else {
        fprintf(stderr, "FIXME(LibC): fopen('%s', '%s')\n", pathname, mode);
        ASSERT_NOT_REACHED();
    }
    //int fd = open(pathname, flags, 0666);
    int fd = open(pathname, flags);
    if (fd < 0)
        return nullptr;
    return make_FILE(fd);
}

int fprintf(FILE* stream, const char* fmt, ...)
{
    char buf[BUFSIZ] = {0};
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buf, fmt, ap);
    int ret = fwrite(buf, total, 1, stream);
    va_end(ap);
    return ret;
}



char* fgets(char* buffer, int size, FILE* stream)
{
    ASSERT(stream);
    ASSERT(size);
    ssize_t nread = 0;
    while (nread < (size - 1)) {
        int ch = fgetc(stream);
        if (ch == EOF)
            break;
        buffer[nread++] = ch;
        if (ch == '\n')
            break;
    }
    if (nread) {
        buffer[nread] = '\0';
        return buffer;
    }
    return nullptr;
}




const char _ctype_[256] = {
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
    (char)(_S | _B), _P, _P, _P, _P, _P, _P, _P,
    _P, _P, _P, _P, _P, _P, _P, _P,
    _N, _N, _N, _N, _N, _N, _N, _N,
    _N, _N, _P, _P, _P, _P, _P, _P,
    _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,
    _U, _U, _U, _U, _U, _U, _U, _U,
    _U, _U, _U, _U, _U, _U, _U, _U,
    _U, _U, _U, _P, _P, _P, _P, _P,
    _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L,
    _L, _L, _L, _L, _L, _L, _L, _L,
    _L, _L, _L, _L, _L, _L, _L, _L,
    _L, _L, _L, _P, _P, _P, _P, _C
};

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c | 0x20;
    return c;
}

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
        return c & ~0x20;
    return c;
}



pid_t getpid(void) {
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}



static char _internal[L_tmpnam];

char * tmpnam(char * s) {
	static int tmp_id = 1;

	if (!s) {
		s = _internal;
	}

	sprintf(s, "/tmp/tmp%d.%d", getpid(), tmp_id++);

	return s;
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
    char* tmp_path = tmpnam(NULL);

    //int fd = open(tmp_path, O_CREAT | O_EXCL | O_RDWR, S_IWUSR | S_IRUSR);
    int fd = open(tmp_path, O_CREAT | O_EXCL | O_RDWR);
    if (fd < 0)
        return nullptr;

    // FIXME: instead of using this hack, implement with O_TMPFILE or similar
    unlink(tmp_path);

    return make_FILE(fd);
}


int ungetc(int c, FILE* stream)
{
    ASSERT(stream);
    if (c == EOF)
        return EOF;
    if (stream->have_ungotten)
        return EOF;
    stream->have_ungotten = true;
    stream->ungotten = c;
    stream->eof = false;
    return c;
}


int setvbuf(FILE* stream, char* buf, int mode, size_t size)
{
    if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF) {
        errno = EINVAL;
        return -1;
    }
    stream->mode = mode;
    if (buf) {
        stream->buffer = buf;
        stream->buffer_size = size;
    } else {
        stream->buffer = stream->default_buffer;
        stream->buffer_size = BUFSIZ;
    }
    stream->buffer_index = 0;
    return 0;
}
}
