/*
 *	babyos/include/syscall_def.h
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
 *  2020-03-12		created
 */



#ifndef _SYSCALL_DEF_H_
#define _SYSCALL_DEF_H_


enum {
    PRINT = 0,
    FORK,
    EXEC,
    MMAP,
    EXIT,
    WAIT,
    SLEEP,
    OPEN,
    CLOSE,
    READ,
    WRITE,
    LSEEK,
    LINK,
    UNLINK,
    MKDIR,
    MKNOD,
    DUP,
    STAT,
    CHDIR,
    PIPE,
    SENDTO,
    RECVFROM,
    SOCKET,
    BIND,
    LISTEN,
    ACCEPT,
    CONNECT,
    SBRK,
    SIGNAL,
    SIGRET,
    KILL,
    PID,
    PS,
    ALARM,
    MAX_SYSCALL,
};


#endif