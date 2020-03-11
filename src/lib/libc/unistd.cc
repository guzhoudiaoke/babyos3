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


uint64 syscall(int id)
{
    int ret;
    __asm__ volatile("int $0x80" : "=a" (ret) : "a" (id));
    return ret;
}

template<typename T1>
uint64 syscall(int id, T1 arg1)
{
    uint64 ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1));

    return ret;
}

template<typename T1, typename T2>
uint64 syscall(int id, T1 arg1, T2 arg2)
{
    uint64 ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2));

    return ret;
}

template<typename T1, typename T2, typename T3>
uint64 syscall(int id, T1 arg1, T2 arg2, T3 arg3)
{
    uint64 ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2), "d" (arg3));

    return ret;
}

template<typename T1, typename T2, typename T3, typename T4>
uint64 syscall(int id, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
    uint64 ret = 0;
    __asm__ volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (id), "D" (arg1), "S" (arg2), "d" (arg3), "c" (arg4));

    return ret;
}

int color_print(uint32 color, const char* str)
{
    return syscall(syscall_t::PRINT, color, str);
}

int fork()
{
    return syscall(syscall_t::FORK);
}

int exec(const char* path, argument_t* arg)
{
    return syscall(syscall_t::EXEC, path, arg);
}

void* mmap(uint64 addr, uint64 len, uint32 prot, uint32 flags)
{
    return (void *) syscall(syscall_t::MMAP, addr, len, prot, flags);
}

void exit(int val)
{
    syscall(syscall_t::EXIT, val);
}

void wait(uint32 pid)
{
    syscall(syscall_t::WAIT, pid);
}

void sleep(uint32 sec)
{
    syscall(syscall_t::SLEEP, sec);
}

int open(const char* path, int mode)
{
    return syscall(syscall_t::OPEN, path, mode);
}

int close(int fd)
{
    return syscall(syscall_t::CLOSE, fd);
}

int64 read(int fd, void* buf, uint64 size)
{
    return syscall(syscall_t::READ, fd, buf, size);
}

int64 write(int fd, void* buf, uint64 size)
{
    return syscall(syscall_t::WRITE, fd, buf, size);
}

int mkdir(const char* path)
{
    return syscall(syscall_t::MKDIR, path);
}

int link(const char* path_old, const char* path_new)
{
    return syscall(syscall_t::LINK, path_old, path_new);
}

int unlink(const char* path)
{
    return syscall(syscall_t::UNLINK, path);
}

int mknod(const char* path, int major, int minor)
{
    return syscall(syscall_t::MKNOD, path, major, minor);
}

int dup(int fd)
{
    return syscall(syscall_t::DUP, fd);
}

int chdir(const char* path)
{
    return syscall(syscall_t::CHDIR, path);
}

int fstat(int fd, stat_t* st)
{
    return syscall(syscall_t::STAT, fd, st);
}

int stat(const char* path, stat_t* st)
{
    int fd = open(path, file_t::MODE_RDONLY);
    if (fd < 0) {
        return -1;
    }

    int ret = fstat(fd, st);
    close(fd);
    return ret;
}

int pipe(int fd[2])
{
    return syscall(syscall_t::PIPE, fd);
}

int socket(int domain, int type, int protocol)
{
    return syscall(syscall_t::SOCKET, domain, type, protocol);
}

int bind(int sockfd, const sock_addr_t* addr)
{
    return syscall(syscall_t::BIND, sockfd, addr);
}

int listen(int sockfd, int backlog)
{
    return syscall(syscall_t::LISTEN, sockfd, backlog);
}

int connect(int sockfd, const sock_addr_t* addr)
{
    
    return syscall(syscall_t::CONNECT, sockfd, addr);
}

int accept(int sockfd, sock_addr_t* addr)
{
    
    return syscall(syscall_t::ACCEPT, sockfd, addr);
}

int send_to(int fd, void *buf, uint32 size, sock_addr_t* addr)
{
    return syscall(syscall_t::SENDTO, fd, buf, size, addr);
}

int recv_from(int fd, void *buf, uint32 size, sock_addr_t* addr)
{
    return syscall(syscall_t::RECVFROM, fd, buf, size, addr);
}

void* sbrk(uint64 increment)
{
    return (void *) syscall(syscall_t::SBRK, increment);
}