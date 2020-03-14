/*
 *	babyos/lib/stdio.h
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


#ifndef _STDIO_H_
#define _STDIO_H_


#include "stddef.h"
#include "unistd.h"
#include <stdarg.h>

#define EOF (-1)

#define BUFSIZ 8192


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define L_tmpnam 256


#define _setjmp(ctx) ({\
    int ret;\
    asm("lea     LJMPRET%=(%%rip), %%rcx\n\t"\
        "xor     %%rax, %%rax\n\t"\
        "mov     %%rbx, (%%rdx)\n\t"\
        "mov     %%rbp, 8(%%rdx)\n\t"\
        "mov     %%r12, 16(%%rdx)\n\t"\
        "mov     %%r13, 24(%%rdx)\n\t"\
        "mov     %%r14, 32(%%rdx)\n\t"\
        "mov     %%r15, 40(%%rdx)\n\t"\
        "mov     %%rsp, 48(%%rdx)\n\t"\
        "mov     %%rcx, 56(%%rdx)\n\t"\
        "LJMPRET%=:\n\t"\
        : "=a" (ret)\
        : "d" (ctx)\
        : "memory", "rcx", "rsi", "rdi", "r8", "r9", "r10", "r11", "cc");\
    ret;\
})
#define longjmp(ctx, x) \
    asm("movq   56(%%rdx), %%rcx\n\t"\
        "movq   48(%%rdx), %%rsp\n\t"\
        "movq   40(%%rdx), %%r15\n\t"\
        "movq   32(%%rdx), %%r14\n\t"\
        "movq   24(%%rdx), %%r13\n\t"\
        "movq   16(%%rdx), %%r12\n\t"\
        "movq   8(%%rdx), %%rbp\n\t"\
        "movq   (%%rdx), %%rbx\n\t"\
        ".cfi_def_cfa %%rdx, 0 \n\t"\
        ".cfi_offset %%rbx, 0 \n\t"\
        ".cfi_offset %%rbp, 8 \n\t"\
        ".cfi_offset %%r12, 16 \n\t"\
        ".cfi_offset %%r13, 24 \n\t"\
        ".cfi_offset %%r14, 32 \n\t"\
        ".cfi_offset %%r15, 40 \n\t"\
        ".cfi_offset %%rsp, 48 \n\t"\
        ".cfi_offset %%rip, 56 \n\t"\
        "jmp    *%%rcx\n\t"\
        : : "d" (ctx), "a" (1))



extern "C" {

struct __STDIO_FILE {
    int fd;
    int eof;
    int error;
    int mode;
    pid_t popen_child;
    char* buffer;
    size_t buffer_size;
    size_t buffer_index;
    int have_ungotten;
    char ungotten;
    char default_buffer[BUFSIZ];
};

typedef struct __STDIO_FILE FILE;

static const int fd_stdin  = 0;
static const int fd_stdout = 1;
static const int fd_error  = 2;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;


void __stdio_init();

int  vsprintf(char *buffer, const char *fmt, va_list args);
int  sprintf(char* buffer, const char* fmt, ...);
int  printf(const char* fmt, ...);
void gets(char* buf, unsigned max);
int puts(const char* buf);





extern FILE * fopen(const char *path, const char *mode);
extern int fclose(FILE * stream);
extern int fseek(FILE * stream, long offset, int whence);
extern long ftell(FILE * stream);
extern FILE * freopen(const char *path, const char *mode, FILE * stream);

extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE * stream);
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE * stream);

extern int fflush(FILE * stream);

extern int sprintf(char *buf, const char *fmt, ...);
extern int fprintf(FILE *stream, const char *fmt, ...);
extern int snprintf(char * buf, size_t size, const char * fmt, ...);
extern int vsnprintf(char * buf, size_t size, const char *fmt, va_list args);
extern int vfprintf(FILE * device, const char *format, va_list ap);
extern int vprintf(const char *format, va_list ap);

extern int fputs(const char *s, FILE *stream);
extern int fputc(int c, FILE *stream);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int fgetc(FILE *stream);
extern int getc(FILE *stream);
extern char *fgets(char *s, int size, FILE *stream);
extern int getchar(void);

extern void rewind(FILE *stream);
extern void setbuf(FILE * stream, char * buf);

extern void perror(const char *s);


extern int feof(FILE * stream);
extern void clearerr(FILE * stream);
extern int ferror(FILE * stream);

extern char * strerror(int errnum);

extern int _fwouldblock(FILE * stream);

extern FILE * tmpfile(void);

extern int setvbuf(FILE * stream, char * buf, int mode, size_t size);

extern int remove(const char * pathname);
extern int rename(const char * oldpath, const char * newpath);


char* tmpnam(char*);
int ungetc(int c, FILE*);

}


#endif
