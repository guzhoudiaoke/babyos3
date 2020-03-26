/*
 *	babyos/lib/unistd.h
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


#ifndef _UNISTD_H_
#define _UNISTD_H_



#include "arg.h"
#include "sock.h"
#include "filemode.h"
#include "stat.h"
#include "signal.h"
#include <stdarg.h>



#define PROT_NONE           0x0       /* page can not be accessed */
#define PROT_READ           0x1       /* page can be read */
#define PROT_WRITE          0x2       /* page can be written */
#define PROT_EXEC           0x4       /* page can be executed */



long syscall(int id);

template<typename T1>
long syscall(int id, T1 arg1);

template<typename T1, typename T2>
long syscall(int id, T1 arg1, T2 arg2);

template<typename T1, typename T2, typename T3>
long syscall(int id, T1 arg1, T2 arg2, T3 arg3);

template<typename T1, typename T2, typename T3, typename T4>
long syscall(int id, T1 arg1, T2 arg2, T3 arg3, T4 arg4);


int   color_print(unsigned color, const char* str);
int   fork();
int   exec(const char* path, argument_t* arg);
void* mmap(long addr, long len, unsigned prot, unsigned flags);
void  exit(int val);
void  wait(unsigned pid);
void  sleep(unsigned sec);

int   open(const char* path, int mode);
int   close(int fd);
long  read(int fd, void* buf, long size);
long  write(int fd, void* buf, long size);
int   mkdir(const char* path);
int   link(const char* path_old, const char* path_new);
int   unlink(const char* path);
int   mknod(const char* path, int major, int minor);
int   dup(int fd);
int   chdir(const char* path);
int   fstat(int fd, stat_t* st);
int   stat(const char* path, stat_t* st);
int   lseek(int fd, unsigned offset, int whence);
int   pipe(int fd[2]);

int   socket(int domain, int type, int protocol);
int   bind(int sockfd, const sock_addr_t* addr);
int   listen(int sockfd, int backlog);
int   connect(int sockfd, const sock_addr_t* addr);
int   accept(int sockfd, sock_addr_t* addr);

int   send_to(int fd, void *buf, unsigned size, sock_addr_t* addr);
int   recv_from(int fd, void *buf, unsigned size, sock_addr_t* addr);
void* sbrk(long increment);


sighandler_t __signal(int signum, sighandler_t handler);

#endif
