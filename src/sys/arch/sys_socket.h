/*
 *	babyos/kernel/sys_socket.h
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
 *  2020-02-26		created
 */




#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_


#include "types.h"
#include "traps.h"
#include "spinlock.h"
#include "socket.h"


class sys_socket_t {
public:
    static void      init();
    //static int32     close_socket(socket_t* socket);

    static uint64    sys_socket(trap_frame_t* frame);
    static uint64    sys_bind(trap_frame_t* frame);
    static uint64    sys_listen(trap_frame_t* frame);
    static uint64    sys_accept(trap_frame_t* frame);
    static uint64    sys_connect(trap_frame_t* frame);

private:
    static int32     socket(uint32 family, uint32 type, uint32 protocol);
    static int32     bind(int fd, sock_addr_t* myaddr);
    static int32     listen(int fd, uint32 backlog);
    static int32     accept(int fd, sock_addr_t* peer_addr);
    static int32     connect(int fd, sock_addr_t* user_addr);

    static socket_t* alloc_socket(uint32 family, uint32 type);
    static int32     release_socket(socket_t* socket);

    static socket_t* look_up_socket(int fd);
};

#endif

