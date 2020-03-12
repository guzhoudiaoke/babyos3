/*
 *	babyos/user/test_socket.cc
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



#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "sock_addr.h"


static void do_server(int sockfd)
{
    int data = 0;
    if (read(sockfd, &data, sizeof(int)) < 0) {
        printf("server read error.\n");
        return;
    }
    printf("server read  %d from client.\n", data);
    data++;

    if (write(sockfd, &data, sizeof(int)) < 0) {
        printf("server write error.\n");
        return;
    }
    printf("server write %d to   client.\n", data);
}

static void socket_server()
{
    int listen_fd = socket(AF_LOCAL, 0, 0);
    if (listen_fd < 0) {
        printf("err, server create socket failed, error %d\n", listen_fd);
        return;
    }
    printf("server create socket succ: %d\n", listen_fd);

    sock_addr_local_t addr;
    addr.m_family = AF_LOCAL;
    strcpy(addr.m_path, "/test_socket");

    int ret = 0;
    if ((ret = bind(listen_fd, &addr)) < 0) {
        printf("err, server bind to %u failed, error %u\n", listen_fd, ret);
        return;
    }
    printf("server bind succ\n");

    if ((ret = listen(listen_fd, 1)) < 0) {
        printf("err, server listen failed, error %u\n", ret);
        return;
    }
    printf("server listen succ\n");

    int conn_fd = -1;
    sock_addr_local_t client_addr;
    for (int i = 0; i < 2; i++) {
        if ((conn_fd = accept(listen_fd, &client_addr)) < 0) {
            printf("accept failed.\n");
            continue;
        }

        printf("server accept success: %u\n", conn_fd);
        if (fork() == 0) {
            close(listen_fd);
            do_server(conn_fd);
            sleep(1);
            exit(0);
        }
        else {
            close(conn_fd);
        }
    }
}

static void do_client(int sockfd, int data)
{
    write(sockfd, &data, sizeof(int));
    printf("client write %d to   server.\n", data);

    read(sockfd, &data, sizeof(int));
    printf("client read  %d from server.\n", data);
}

static void socket_client(int data)
{
    int sock_fd = socket(AF_LOCAL, 0, 0);
    if (sock_fd < 0) {
        printf("client create socket failed, error %u\n", sock_fd);
        return;
    }
    printf("client create socket success, fd: %u\n", sock_fd);


    sock_addr_local_t addr;
    addr.m_family = AF_LOCAL;
    strcpy(addr.m_path, "/test_socket");

    int ret = 0;
    if ((ret = connect(sock_fd, &addr)) < 0) {
        printf("client connect to fd: %u failed, error %u\n", sock_fd, ret);
        return;
    }

    printf("client connect success\n");
    do_client(sock_fd, data);
}

int main(int argc, char** argv)
{
    int pid1 = -1;
    int pid2 = -1;
    int pid3 = -1;

    pid1 = fork();
    if (pid1 == 0) {
        /* server */
        socket_server();
        exit(0);
    }

    sleep(1);
    pid2 = fork();
    if (pid2 == 0) {
        /* client */
        socket_client(1234);
        sleep(1);
        exit(0);
    }

    sleep(1);
    pid3 = fork();
    if (pid3 == 0) {
        /* client */
        socket_client(5678);
        sleep(1);
        exit(0);
    }

    /* shell */
    wait(pid1);
    wait(pid2);
    wait(pid3);

    exit(0);
    return 0;
}

