/*
 *	babyos/kernel/socket.h
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





#ifndef _SOCKET_H_
#define _SOCKET_H_


#include "types.h"
#include "sem.h"
#include "sock_addr.h"
#include "sock_ring_buffer.h"
#include "errno.h"
#include "sock.h"


class socket_t {
public:
    typedef enum sock_type_e {
        SOCK_STREAM = 1,
        SOCK_DGRAM,
        SOCK_RAW,
    } sock_type_t;

    typedef enum protocol_e {
        PROTO_ICMP = 1,
        PROTO_TCP = 6,
        PROTO_UDP = 17,
    } protocol_t;

    typedef enum socket_state_e {
        SS_FREE = 0,
        SS_UNCONNECTED,
        SS_CONNECTING,
        SS_CONNECTED,
        SS_DISCONNECTING,
    } socket_state_t;

    typedef enum socket_flag_e {
        SF_ACCEPTCON = (1 << 16),   /* performed a listen */
        SF_WAITDATA  = (1 << 17),   /* wait data to read */
        SF_NOSPACE   = (1 << 18),   /* no space to write */
    } socket_flag_t;

    virtual int32 create(uint32 family, uint32 type, uint32 protocol);
    virtual int32 release() = 0;
    virtual int32 dup(socket_t* socket) = 0;
    virtual int32 get_addr(sock_addr_t* addr) = 0;

    virtual int32 bind(sock_addr_t* myaddr) = 0;
    virtual int32 listen(uint32 backlog) = 0;
    virtual int32 accept(socket_t* server_socket) = 0;
    virtual int32 connect(sock_addr_t* user_addr) = 0;

    virtual int32 read(void* buf, uint64 size) = 0;
    virtual int32 write(void* buf, uint64 size) = 0;
    virtual int32 send_to(void *buf, uint32 size, sock_addr_t* addr_to) = 0;
    virtual int32 recv_from(void *buf, uint32 size, sock_addr_t* addr_from) = 0;

public:
    uint32              m_flags;
    uint32              m_family;
    uint32              m_type;
    uint32              m_protocol;
    socket_state_t      m_state;
};

#endif

