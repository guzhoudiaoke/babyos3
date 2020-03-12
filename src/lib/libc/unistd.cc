/*
 *	babyos/lib/unistd.cc
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


#include "unistd.h"
#include "syscall_def.h"


long syscall(int id)
{
    int ret;
    __asm__ volatile("int $0x80" : "=a" (ret) : "a" (id));
    return ret;
}

template<typename T1>
long syscall(int id, T1 arg1)
{
    long ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1));

    return ret;
}

template<typename T1, typename T2>
long syscall(int id, T1 arg1, T2 arg2)
{
    long ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2));

    return ret;
}

template<typename T1, typename T2, typename T3>
long syscall(int id, T1 arg1, T2 arg2, T3 arg3)
{
    long ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2), "d" (arg3));

    return ret;
}

template<typename T1, typename T2, typename T3, typename T4>
long syscall(int id, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
    long ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2), "d" (arg3), "c" (arg4));

    return ret;
}

int color_print(unsigned color, const char* str)
{
    return syscall(PRINT, color, str);
}

int fork()
{
    return syscall(FORK);
}

int exec(const char* path, argument_t* arg)
{
    return syscall(EXEC, path, arg);
}

void* mmap(long addr, unsigned long len, unsigned prot, unsigned flags)
{
    return (void *) syscall(MMAP, addr, len, prot, flags);
}

void exit(int val)
{
    syscall(EXIT, val);
}

void wait(unsigned pid)
{
    syscall(WAIT, pid);
}

void sleep(unsigned sec)
{
    syscall(SLEEP, sec);
}

int open(const char* path, int mode)
{
    return syscall(OPEN, path, mode);
}

int close(int fd)
{
    return syscall(CLOSE, fd);
}

long read(int fd, void* buf, long size)
{
    return syscall(READ, fd, buf, size);
}

long write(int fd, void* buf, long size)
{
    return syscall(WRITE, fd, buf, size);
}

int mkdir(const char* path)
{
    return syscall(MKDIR, path);
}

int link(const char* path_old, const char* path_new)
{
    return syscall(LINK, path_old, path_new);
}

int unlink(const char* path)
{
    return syscall(UNLINK, path);
}

int mknod(const char* path, int major, int minor)
{
    return syscall(MKNOD, path, major, minor);
}

int dup(int fd)
{
    return syscall(DUP, fd);
}

int chdir(const char* path)
{
    return syscall(CHDIR, path);
}

int fstat(int fd, stat_t* st)
{
    return syscall(STAT, fd, st);
}

int stat(const char* path, stat_t* st)
{
    int fd = open(path, MODE_RDONLY);
    if (fd < 0) {
        return -1;
    }

    int ret = fstat(fd, st);
    close(fd);
    return ret;
}

int pipe(int fd[2])
{
    return syscall(PIPE, fd);
}

int socket(int domain, int type, int protocol)
{
    return syscall(SOCKET, domain, type, protocol);
}

int bind(int sockfd, const sock_addr_t* addr)
{
    return syscall(BIND, sockfd, addr);
}

int listen(int sockfd, int backlog)
{
    return syscall(LISTEN, sockfd, backlog);
}

int connect(int sockfd, const sock_addr_t* addr)
{
    
    return syscall(CONNECT, sockfd, addr);
}

int accept(int sockfd, sock_addr_t* addr)
{
    
    return syscall(ACCEPT, sockfd, addr);
}

int send_to(int fd, void *buf, unsigned size, sock_addr_t* addr)
{
    return syscall(SENDTO, fd, buf, size, addr);
}

int recv_from(int fd, void *buf, unsigned size, sock_addr_t* addr)
{
    return syscall(RECVFROM, fd, buf, size, addr);
}

void* sbrk(long increment)
{
    return (void *) syscall(SBRK, increment);
}