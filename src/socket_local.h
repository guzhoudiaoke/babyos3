/*
 *	babyos/kernel/socket_local.h
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




#ifndef _SOCKET_LOCAL_H_
#define _SOCKET_LOCAL_H_


#include "types.h"
#include "socket.h"
#include "spinlock.h"


#define MAX_LOCAL_SOCKET    128


class socket_local_t : public socket_t {
public:
    socket_local_t();
    void  init();

    int32 create(uint32 family, uint32 type, uint32 protocol);
    int32 get_addr(sock_addr_t* addr);
    int32 release();
    int32 dup(socket_t* socket);

    int32 bind(sock_addr_t* myaddr);
    int32 listen(uint32 backlog);
    int32 accept(socket_t* server_socket);
    int32 connect(sock_addr_t* user_addr);
    int32 read(void* buf, uint64 size);
    int32 write(void* buf, uint64 size);
    int32 send_to(void *buf, uint32 size, sock_addr_t* addr_to);
    int32 recv_from(void *buf, uint32 size, sock_addr_t* addr_from);


    static void             init_local_sockets();
    static socket_t*        alloc_local_socket();
    static void             release_local_socket(socket_t* socket);
    static socket_local_t*  lookup_local_socket(sock_addr_local_t* addr);
    static int32            bind_local_socket(socket_local_t* socket, sock_addr_local_t* addr);

public:
    uint32                    m_ref;
    sock_addr_local_t         m_addr;
    sock_ring_buffer_t        m_sock_buf;

    list_t<socket_local_t *>  m_connecting_list;
    spinlock_t                m_connecting_list_lock;
    socket_local_t*           m_connected_socket;
    semaphore_t               m_wait_connect_sem;
    semaphore_t               m_wait_accept_sem;

    static spinlock_t         s_lock;
    static socket_local_t     s_local_sockets[MAX_LOCAL_SOCKET];
};

#endif
