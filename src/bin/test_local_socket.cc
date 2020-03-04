/*
 *	babyos/user/test_local_socket.cc
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
 *  2020-02-20		created
 */



#include "shell.h"
#include "userlib.h"
#include "file.h"
#include "types.h"
//#include "socket_local.h"


static void do_server(int sockfd)
{
    int data = 0;
    if (userlib_t::read(sockfd, &data, sizeof(int)) < 0) {
        userlib_t::printf("server read error.\n");
        return;
    }
    userlib_t::printf("server read  %d from client.\n", data);
    data++;

    if (userlib_t::write(sockfd, &data, sizeof(int)) < 0) {
        userlib_t::printf("server write error.\n");
        return;
    }
    userlib_t::printf("server write %d to   client.\n", data);
}

static void socket_server()
{
    int listen_fd = userlib_t::socket(socket_t::AF_LOCAL, 0, 0);
    if (listen_fd < 0) {
        userlib_t::printf("err, server create socket failed, error %d\n", listen_fd);
        return;
    }
    userlib_t::printf("server create socket succ: %d\n", listen_fd);

    sock_addr_local_t addr;
    addr.m_family = socket_t::AF_LOCAL;
    userlib_t::strcpy(addr.m_path, "/test_socket");

    int ret = 0;
    if ((ret = userlib_t::bind(listen_fd, &addr)) < 0) {
        userlib_t::printf("err, server bind to %u failed, error %u\n", listen_fd, ret);
        return;
    }
    userlib_t::printf("server bind succ\n");

    if ((ret = userlib_t::listen(listen_fd, 1)) < 0) {
        userlib_t::printf("err, server listen failed, error %u\n", ret);
        return;
    }
    userlib_t::printf("server listen succ\n");

    int conn_fd = -1;
    sock_addr_local_t client_addr;
    for (int i = 0; i < 2; i++) {
        if ((conn_fd = userlib_t::accept(listen_fd, &client_addr)) < 0) {
            userlib_t::printf("accept failed.\n");
            continue;
        }

        userlib_t::printf("server accept success: %u\n", conn_fd);
        if (userlib_t::fork() == 0) {
            userlib_t::close(listen_fd);
            do_server(conn_fd);
            userlib_t::sleep(1);
            userlib_t::exit(0);
        }
        else {
            userlib_t::close(conn_fd);
        }
    }
}

static void do_client(int sockfd, int data)
{
    userlib_t::write(sockfd, &data, sizeof(int));
    userlib_t::printf("client write %d to   server.\n", data);

    userlib_t::read(sockfd, &data, sizeof(int));
    userlib_t::printf("client read  %d from server.\n", data);
}

static void socket_client(int data)
{
    int sock_fd = userlib_t::socket(socket_t::AF_LOCAL, 0, 0);
    if (sock_fd < 0) {
        userlib_t::printf("client create socket failed, error %u\n", sock_fd);
        return;
    }
    userlib_t::printf("client create socket success, fd: %u\n", sock_fd);


    sock_addr_local_t addr;
    addr.m_family = socket_t::AF_LOCAL;
    userlib_t::strcpy(addr.m_path, "/test_socket");

    int ret = 0;
    if ((ret = userlib_t::connect(sock_fd, &addr)) < 0) {
        userlib_t::printf("client connect to fd: %u failed, error %u\n", sock_fd, ret);
        return;
    }

    userlib_t::printf("client connect success\n");
    do_client(sock_fd, data);
}

int main(int argc, char** argv)
{
    int32 pid1 = -1;
    int32 pid2 = -1;
    int32 pid3 = -1;

    pid1 = userlib_t::fork();
    if (pid1 == 0) {
        /* server */
        socket_server();
        userlib_t::exit(0);
    }

    userlib_t::sleep(1);
    pid2 = userlib_t::fork();
    if (pid2 == 0) {
        /* client */
        socket_client(1234);
        userlib_t::sleep(1);
        userlib_t::exit(0);
    }

    userlib_t::sleep(1);
    pid3 = userlib_t::fork();
    if (pid3 == 0) {
        /* client */
        socket_client(5678);
        userlib_t::sleep(1);
        userlib_t::exit(0);
    }

    /* shell */
    userlib_t::wait(pid1);
    userlib_t::wait(pid2);
    userlib_t::wait(pid3);

    userlib_t::exit(0);
    return 0;
}

