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

typedef struct _FILE {
    int fd;
    int offset;
} FILE;


static const int fd_stdin  = 0;
static const int fd_stdout = 1;
static const int fd_error  = 2;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;


int  vsprintf(char *buffer, const char *fmt, va_list args);
int  sprintf(char* buffer, const char* fmt, ...);
int  printf(const char* fmt, ...);
void gets(char* buf, unsigned max);
void puts(char* buf);





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


#endif
