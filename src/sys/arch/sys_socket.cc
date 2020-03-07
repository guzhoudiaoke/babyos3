/*
 *	babyos/kernel/sys_socket.cc
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



#include "syscall.h"
#include "sys_socket.h"
#include "babyos.h"
#include "socket_local.h"
//#include "socket_raw.h"
//#include "socket_dgram.h"
//#include "socket_stream.h"


/******************************************************************************/

void sys_socket_t::init()
{
    socket_local_t::init_local_sockets();
    //socket_raw_t::init_raw_sockets();
    //socket_dgram_t::init_dgram_sockets();
    //socket_stream_t::init_stream_sockets();
}

socket_t* sys_socket_t::alloc_socket(uint32 family, uint32 type)
{
    if (family == socket_t::AF_LOCAL) {
        return socket_local_t::alloc_local_socket();
    }
    else if (family == socket_t::AF_INET) {
        if (type == socket_t::SOCK_RAW) {
            //return socket_raw_t::alloc_raw_socket();
        }
        else if (type == socket_t::SOCK_DGRAM) {
            //return socket_dgram_t::alloc_dgram_socket();
        }
        else if (type == socket_t::SOCK_STREAM) {
            //return socket_stream_t::alloc_stream_socket();
        }
    }

    return NULL;
}

int32 sys_socket_t::release_socket(socket_t* socket)
{
    if (socket != NULL) {
        socket->release();
    }
    return 0;
}

socket_t* sys_socket_t::look_up_socket(int fd)
{
    socket_t* socket = NULL;
    if (fd < 0 || fd >= MAX_OPEN_FILE) {
        return socket;
    }

    file_t* file = current->get_file(fd);
    if (file == NULL) {
        return socket;
    }

    socket = file->m_socket;
    return socket;
}

int32 sys_socket_t::socket(uint32 family, uint32 type, uint32 protocol)
{
    if (family >= socket_t::AF_MAX) {
        return -EINVAL;
    }

    /* alloc a socket */
    socket_t* socket = alloc_socket(family, type);
    if (socket == NULL) {
        return -ENOSR;
    }
    socket->create(family, type, protocol);

    /* alloc a file */
    file_t* file = os()->fs()->alloc_file();
    if (file == NULL) {
        release_socket(socket);
        return -ENOSR;
    }
    file->init(file_t::TYPE_SOCKET, socket);

    /* alloc a fd and bind to file */
    int fd = current->alloc_fd(file);
    if (fd < 0) {
        os()->fs()->close_file(file);
        release_socket(socket);
        return -ENOSR;
    }

    return fd;
}

int32 sys_socket_t::bind(int fd, sock_addr_t* myaddr)
{
    socket_t* socket = look_up_socket(fd);
    if (socket == NULL) {
        return -EBADF;
    }

    return socket->bind(myaddr);
}

int32 sys_socket_t::listen(int fd, uint32 backlog)
{
    socket_t* socket = look_up_socket(fd);
    if (socket == NULL) {
        return -EBADF;
    }

    if (socket->m_state != socket_t::SS_UNCONNECTED) {
        return -EINVAL;
    }

    socket->listen(backlog);
    socket->m_flags |= socket_t::SF_ACCEPTCON;

    return 0;
}

int32 sys_socket_t::accept(int fd, sock_addr_t* client_addr)
{
    socket_t* socket = look_up_socket(fd);

    /* not find a socket */
    if (socket == NULL) {
        return -EBADF;
    }

    /* not connected */
    if (socket->m_state != socket_t::SS_UNCONNECTED) {
        return -EINVAL;
    }

    /* not listening */
    if (!(socket->m_flags & socket_t::SF_ACCEPTCON)) {
        return -EINVAL;
    }

    /* alloc a new socket to accept the connect */
    socket_t* new_socket = alloc_socket(socket->m_family, socket->m_type);
    if (new_socket == NULL) {
        return -ENOSR;
    }
    new_socket->dup(socket);

    /* alloc a file */
    file_t* file = os()->fs()->alloc_file();
    if (file == NULL) {
        release_socket(new_socket);
        return -ENOSR;
    }
    file->init(file_t::TYPE_SOCKET, new_socket);

    /* alloc a fd and bind to file */
    int new_fd = current->alloc_fd(file);
    if (new_fd < 0) {
        os()->fs()->close_file(file);
        release_socket(new_socket);
        return -ENOSR;
    }

    uint32 ret = new_socket->accept(socket);
    if (ret < 0) {
        return ret;
    }

    if (client_addr != NULL) {
        if (new_socket->get_addr(client_addr) < 0) {
            return -ECONNABORTED;
        }
    }

    return new_fd;
}

int32 sys_socket_t::connect(int fd, sock_addr_t* user_addr)
{
    socket_t* socket = look_up_socket(fd);
    if (socket == NULL) {
        return -EBADF;
    }

    switch (socket->m_state) {
    case socket_t::SS_UNCONNECTED:
        break;
    case socket_t::SS_CONNECTED:
        return -EISCONN;
    case socket_t::SS_CONNECTING:
        break;

    default:
        return -EINVAL;
    }

    return socket->connect(user_addr);
}


/* socket syscalls */
uint64 sys_socket_t::sys_socket(trap_frame_t* frame)
{
    uint32 family = (uint32) syscall_t::get_argument(frame, 0);
    uint32 type = (uint32) syscall_t::get_argument(frame, 1);
    uint32 protocol = (uint32) syscall_t::get_argument(frame, 2);
    return socket(family, type, protocol);
}

uint64 sys_socket_t::sys_bind(trap_frame_t* frame)
{
    int fd = (int) syscall_t::get_argument(frame, 0);
    sock_addr_t* myaddr = (sock_addr_t *) syscall_t::get_argument(frame, 1);
    return bind(fd, myaddr);
}

uint64 sys_socket_t::sys_listen(trap_frame_t* frame)
{
    int fd = (int) syscall_t::get_argument(frame, 0);
    uint32 backlog = (uint32) syscall_t::get_argument(frame, 1);
    return listen(fd, backlog);
}

uint64 sys_socket_t::sys_accept(trap_frame_t* frame)
{
    int fd = (int) syscall_t::get_argument(frame, 0);
    sock_addr_t* peer_addr = (sock_addr_t *) syscall_t::get_argument(frame, 1);
    return accept(fd, peer_addr);
}

uint64 sys_socket_t::sys_connect(trap_frame_t* frame)
{
    int fd = (int) syscall_t::get_argument(frame, 0);
    sock_addr_t* user_addr = (sock_addr_t *) syscall_t::get_argument(frame, 1);
    return connect(fd, user_addr);
}

//int32 sys_socket_t::close_socket(socket_t* socket)
//{
//    return release_socket(socket);
//}
//
