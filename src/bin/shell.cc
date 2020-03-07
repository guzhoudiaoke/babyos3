/*
 *	babyos/user/shell.cc
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
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "file.h"
#include "color.h"


#define MAX_CMD_LEN 128


argument_t argument;

const char* parse_cmd(const char* cmd_line, char* cmd)
{
    memset(cmd, 0, MAX_CMD_LEN);
    if (*cmd_line != '/') {
        strcpy(cmd, "/bin/");
        cmd += strlen(cmd);
    }

    const char* p = cmd_line;
    while (*p != ' ' && *p != '\0') {
        *cmd++ = *p++;
    }

    while (*p == ' ') {
        p++;
    }

    return p;
}

void parse_cmd_line(const char* cmd_line, char* cmd, argument_t* arg)
{
    const char* p = parse_cmd(cmd_line, cmd);
    arg->m_argc = 0;
    char* q = arg->m_argv[arg->m_argc++];
    strcpy(q, cmd);

    if (*p == '\0') {
        return;
    }

    while (true) {
        char* q = arg->m_argv[arg->m_argc];
        memset(q, 0, MAX_ARG_LEN);
        while (*p != ' ' && *p != '\0') {
            *q++ = *p++;
        }

        while (*p == ' ') {
            p++;
        }

        arg->m_argc++;
        if (*p == '\0') {
            break;
        }
    }
}

static char command[MAX_CMD_LEN] = {0};
void do_cmd(const char* cmd_line)
{
    parse_cmd_line(cmd_line, command, &argument);
    int32 pid = fork();
    //printf("shell fork return: %d\n", pid);
    if (pid == 0) {
        int ret = exec(command, &argument);
        printf("ERROR: shell exec returned: %d\n", pid);
        if (ret < 0) {
            exit(-1);
        }
    }

    //printf("will wait child\n");
    wait(pid);
    //printf("after wait child\n");
}

// only for test
static void test_fork_exec_wait_exit(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    argument.m_argc = 1;
    argument.m_argv[0][0] = 0;
    for (int i = 0; i < t; i++) {
        int32 pid = fork();
        if (pid == 0) {
            int ret = exec("/bin/ls", &argument);
            if (ret < 0) {
                exit(-1);
            }
            exit(0);
        }

        wait(pid);
    }
}

// only for test
static void test_fork_wait_exit(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    argument.m_argc = 1;
    argument.m_argv[0][0] = 0;
    for (int i = 0; i < t; i++) {
        int32 pid = fork();
        if (pid == 0) {
            //if (i % 100 == 0) {
                printf("%u\n", i);
            //}
            exit(0);
        }

        wait(pid);
    }
}

static void test_pipe(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    while (t--) {
        int fd[2];
        if (pipe(fd) < 0) {
            printf("failed to create pipe\n");
            return;
        }

        printf("succ to create pipe: %d, %d\n", fd[0], fd[1]);

        int32 pid = fork();
        if (pid == 0) {
            close(fd[0]);

            char ch = 'a';
            for (int i = 0; i < 5; i++, ch++) {
                write(fd[1], &ch, 1);
                printf("child write %c to pipe\n", ch);
                //sleep(1);
            }
            exit(0);
        }
        else {
            close(fd[1]);

            char ch = '\0';
            for (int i = 0; i < 5; i++) {
                read(fd[0], &ch, 1);
                printf("parent read %c from pipe\n", ch);
            }
            close(fd[0]);
            wait(pid);
        }
    }
}

/********************* socket local *******************/

//static void do_server(int sockfd)
//{
//    int data = 0;
//    if (read(sockfd, &data, sizeof(int)) < 0) {
//        printf("server read error.\n");
//        return;
//    }
//    printf("server read  %d from client.\n", data);
//    data++;
//
//    if (write(sockfd, &data, sizeof(int)) < 0) {
//        printf("server write error.\n");
//        return;
//    }
//    printf("server write %d to   client.\n", data);
//}
//
//static void socket_server()
//{
//    int listen_fd = socket(socket_t::AF_LOCAL, 0, 0);
//    if (listen_fd < 0) {
//        printf("err, server create socket failed, error %d\n", listen_fd);
//        return;
//    }
//    printf("server create socket succ: %d\n", listen_fd);
//
//    sock_addr_local_t addr;
//    addr.m_family = socket_t::AF_LOCAL;
//    strcpy(addr.m_path, "/test_socket");
//
//    int ret = 0;
//    if ((ret = bind(listen_fd, &addr)) < 0) {
//        printf("err, server bind to %u failed, error %u\n", listen_fd, ret);
//        return;
//    }
//    printf("server bind succ\n");
//
//    if ((ret = listen(listen_fd, 1)) < 0) {
//        printf("err, server listen failed, error %u\n", ret);
//        return;
//    }
//    printf("server listen succ\n");
//
//    int conn_fd = -1;
//    sock_addr_local_t client_addr;
//    for (int i = 0; i < 2; i++) {
//        if ((conn_fd = accept(listen_fd, &client_addr)) < 0) {
//            printf("accept failed.\n");
//            continue;
//        }
//
//        printf("server accept success: %u\n", conn_fd);
//        if (fork() == 0) {
//            close(listen_fd);
//            do_server(conn_fd);
//            sleep(1);
//            exit(0);
//        }
//        else {
//            close(conn_fd);
//        }
//    }
//}
//
//static void do_client(int sockfd, int data)
//{
//    write(sockfd, &data, sizeof(int));
//    printf("client write %d to   server.\n", data);
//
//    read(sockfd, &data, sizeof(int));
//    printf("client read  %d from server.\n", data);
//}
//
//static void socket_client(int data)
//{
//    int sock_fd = socket(socket_t::AF_LOCAL, 0, 0);
//    if (sock_fd < 0) {
//        printf("client create socket failed, error %u\n", sock_fd);
//        return;
//    }
//    printf("client create socket success, fd: %u\n", sock_fd);
//
//
//    sock_addr_local_t addr;
//    addr.m_family = socket_t::AF_LOCAL;
//    strcpy(addr.m_path, "/test_socket");
//
//    int ret = 0;
//    if ((ret = connect(sock_fd, &addr)) < 0) {
//        printf("client connect to fd: %u failed, error %u\n", sock_fd, ret);
//        return;
//    }
//
//    printf("client connect success\n");
//    do_client(sock_fd, data);
//}
//
//static void test_socket()
//{
//    int32 pid1 = -1;
//    int32 pid2 = -1;
//    int32 pid3 = -1;
//
//    pid1 = fork();
//    if (pid1 == 0) {
//        /* server */
//        socket_server();
//        exit(0);
//    }
//
//    sleep(1);
//    pid2 = fork();
//    if (pid2 == 0) {
//        /* client */
//        socket_client(1234);
//        sleep(1);
//        exit(0);
//    }
//
//    sleep(1);
//    pid3 = fork();
//    if (pid3 == 0) {
//        /* client */
//        socket_client(5678);
//        sleep(1);
//        exit(0);
//    }
//
//    /* shell */
//    wait(pid1);
//    wait(pid2);
//    wait(pid3);
//}

/********************* socket local *******************/

/************************ udp ************************/
//const int TEST_UDP_PORT = 12345;
//static void udp_server()
//{
//    int sock_fd = socket(socket_t::AF_INET, socket_t::SOCK_DGRAM, 0);
//    if (sock_fd < 0) {
//        printf("ERROR, server create socket failed, error %u\n", sock_fd);
//        return;
//    }
//    printf("server create socket success, fd: %u\n", sock_fd);
//
//    sock_addr_inet_t addr;
//    addr.m_family = socket_t::AF_INET;
//    addr.m_ip = htonl(sock_addr_inet_t::INADDR_ANY);
//    addr.m_port = htons(TEST_UDP_PORT);
//    if (bind(sock_fd, &addr) < 0) {
//        printf("ERROR, server bind failed\n");
//        return;
//    }
//    printf("server bind success\n");
//
//    char buffer[512] = {0};
//    for (; ; ) {
//        memset(buffer, 0, 512);
//        sock_addr_inet_t addr_client;
//        int ret = recv_from(sock_fd, buffer, 512, &addr_client);
//        if (ret < 0) {
//            printf("ERROR, failed to recv_from, error %u\n", ret);
//            break;
//        }
//
//        uint32 ip = ntohl(addr_client.m_ip);
//        uint8* p = (uint8 *) &ip;
//        printf("server receive from %u.%u.%u.%u, %u: %s\n", 
//                p[3], p[2], p[1], p[0], ntohs(addr_client.m_port), buffer);
//
//        ret = send_to(sock_fd, buffer, strlen(buffer), &addr_client);
//        if (ret < 0) {
//            printf("ERROR, failed to send_to, error %u\n", ret);
//            break;
//        }
//    }
//}
//
//static void test_udp_server()
//{
//    int32 pid = fork();
//    if (pid == 0) {
//        /* server */
//        udp_server();
//        exit(0);
//    }
//
//    /* shell */
//    wait(pid);
//}
//
//static void udp_client()
//{
//    int sock_fd = socket(socket_t::AF_INET, socket_t::SOCK_DGRAM, 0);
//    if (sock_fd < 0) {
//        printf("ERROR, client create socket failed, error %u\n", sock_fd);
//        return;
//    }
//    printf("client create socket success, fd: %u\n", sock_fd);
//
//    sock_addr_inet_t addr;
//    addr.m_family = socket_t::AF_INET;
//    addr.m_ip = htonl(make_ipaddr(192, 168, 1, 105));
//    addr.m_port = htons(TEST_UDP_PORT);
//
//    char buffer[512] = {0};
//    for (int i = 0; i < 5; i++) {
//        memset(buffer, 0, 512);
//        gets(buffer, 512);
//        int ret = send_to(sock_fd, buffer, strlen(buffer), &addr);
//        if (ret < 0) {
//            printf("ERROR, failed to send_to, error %u\n", ret);
//            break;
//        }
//
//        memset(buffer, 0, 512);
//        sock_addr_inet_t addr_recv;
//        ret = recv_from(sock_fd, buffer, 512, &addr_recv);
//        if (ret < 0) {
//            printf("ERROR, failed to recv_from, error %u\n", ret);
//            break;
//        }
//
//        printf("receive: %s\n", buffer);
//    }
//}
//
//static void test_udp_client()
//{
//    int32 pid = fork();
//    if (pid == 0) {
//        /* server */
//        udp_client();
//        exit(0);
//    }
//
//    /* shell */
//    wait(pid);
//}

/************************ udp ************************/


/************************ nslookup ************************/

//void ns_lookup(const char* name)
//{
//    uint32 ip = get_ip_by_name(name);
//    uint8* p = (uint8 *) &ip;
//    printf("IP: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
//}

/************************ nslookup ************************/

/************************ tcp ************************/
//static void tcp_server_echo(int conn_fd)
//{
//    char buffer[512] = {0};
//    while (1) {
//        memset(buffer, 0, 512);
//        int ret = read(conn_fd, buffer, 512);
//        if (ret < 0) {
//            printf("ERROR, failed to read, error %u\n", ret);
//            break;
//        }
//        printf("server read %d bytes from client: %s\n", ret, buffer);
//
//        ret = write(conn_fd, buffer, strlen(buffer));
//        if (ret < 0) {
//            printf("ERROR, failed to write, error %u\n", ret);
//            break;
//        }
//        printf("server write %d bytes to client.\n", ret);
//    }
//    printf("break from while\n");
//}

//const int TEST_TCP_PORT = 56789;
//static void tcp_server()
//{
//    int sock_fd = socket(socket_t::AF_INET, socket_t::SOCK_STREAM, 0);
//    if (sock_fd < 0) {
//        printf("ERROR, server create socket failed, error %u\n", sock_fd);
//        return;
//    }
//    printf("server create socket success, fd: %u\n", sock_fd);
//
//    sock_addr_inet_t addr;
//    addr.m_family = socket_t::AF_INET;
//    addr.m_ip = htonl(sock_addr_inet_t::INADDR_ANY);
//    addr.m_port = htons(TEST_TCP_PORT);
//
//    int ret = 0;
//    if ((ret = bind(sock_fd, &addr)) < 0) {
//        printf("ERROR, server bind failed: %u\n", ret);
//        return;
//    }
//    printf("server bind success\n");
//
//    if ((ret = listen(sock_fd, 5)) < 0) {
//        printf("err, server listen failed, error %u\n", ret);
//        return;
//    }
//    printf("server listen succ\n");
//
//    int conn_fd = -1;
//    sock_addr_local_t client_addr;
//    for (int i = 0; i < 5; i++) {
//        if ((conn_fd = accept(sock_fd, &client_addr)) < 0) {
//            printf("accept failed %u\n", -conn_fd);
//            continue;
//        }
//        printf("server accept success: %u\n", conn_fd);
//        if (fork() == 0) {
//            close(sock_fd);
//            tcp_server_echo(conn_fd);
//            //close(conn_fd);
//            exit(0);
//        }
//        else {
//            close(conn_fd);
//        }
//    }
//}

//static void tcp_client()
//{
//    int sock_fd = socket(socket_t::AF_INET, socket_t::SOCK_STREAM, 0);
//    if (sock_fd < 0) {
//        printf("ERROR, client create socket failed, error %u\n", -sock_fd);
//        return;
//    }
//    printf("client create socket success, fd: %u\n", sock_fd);
//
//    sock_addr_inet_t addr;
//    addr.m_family = socket_t::AF_INET;
//    addr.m_ip = htonl(make_ipaddr(192, 168, 1, 105));
//    addr.m_port = htons(TEST_TCP_PORT);
//
//    int ret = 0;
//    if ((ret = connect(sock_fd, &addr)) < 0) {
//        printf("client connect to fd: %u failed, error %u\n", sock_fd, -ret);
//        return;
//    }
//
//    printf("client connect success\n");
//    char buffer[512] = {0};
//    for (int i = 0; i < 1; i++) {
//        memset(buffer, 0, 512);
//        printf("input some data: ");
//        gets(buffer, 512);
//
//        ret = write(sock_fd, buffer, strlen(buffer));
//        if (ret < 0) {
//            printf("ERROR, failed to write, error %u\n", ret);
//            break;
//        }
//        printf("client write %d bytes to server.\n", ret);
//
//        ret = read(sock_fd, buffer, ret);
//        if (ret < 0) {
//            printf("ERROR, failed to read, error %u\n", ret);
//            break;
//        }
//        printf("client read %d bytes from server: %s\n", ret, buffer);
//    }
//    close(sock_fd);
//}
//
//static void test_tcp_client()
//{
//    int32 pid = fork();
//    if (pid == 0) {
//        /* server */
//        tcp_client();
//        exit(0);
//    }
//
//    /* shell */
//    wait(pid);
//}
//
//static void test_tcp_server()
//{
//    int32 pid = fork();
//    if (pid == 0) {
//        /* server */
//        tcp_server();
//        exit(0);
//    }
//
//    /* shell */
//    wait(pid);
//}

/************************ tcp ************************/

int main()
{
    char cmd_line[MAX_CMD_LEN] = {0};
    printf("This is printed by shell.\n");

    while (true) {
        color_print(GREEN, "liuruyi $ ");
        gets(cmd_line, MAX_CMD_LEN);
        if (strlen(cmd_line) == 0) {
            continue;
        }

        if (strncmp(cmd_line, "cd ", 3) == 0) {
            if (chdir(cmd_line + 3) < 0) {
                printf("can't cd %s\n", cmd_line+3);
            }
            continue;
        }

        /* used for test */
        if (strncmp(cmd_line, "test ", 5) == 0) {
            test_fork_exec_wait_exit(cmd_line + 5);
            continue;
        }
        if (strncmp(cmd_line, "test2 ", 6) == 0) {
            test_fork_wait_exit(cmd_line + 6);
            continue;
        }
        if (strncmp(cmd_line, "testpipe ", 9) == 0) {
            test_pipe(cmd_line + 9);
            continue;
        }
        //if (strncmp(cmd_line, "testsocket", 10) == 0) {
        //    test_socket();
        //    continue;
        //}
        //if (strncmp(cmd_line, "testudpserver", 13) == 0) {
        //    test_udp_server();
        //    continue;
        //}
        //if (strncmp(cmd_line, "testudpclient", 13) == 0) {
        //    test_udp_client();
        //    continue;
        //}
        //if (strncmp(cmd_line, "nslookup", 8) == 0) {
        //    ns_lookup(cmd_line + 9);
        //    continue;
        //}
        //if (strncmp(cmd_line, "tcpserver", 9) == 0) {
        //    test_tcp_server();
        //    continue;
        //}
        //if (strncmp(cmd_line, "tcpclient", 9) == 0) {
        //    test_tcp_client();
        //    continue;
        //}
        //if (strncmp(cmd_line, "ts", 2) == 0) {
        //    test_tcp_server();
        //    continue;
        //}
        //if (strncmp(cmd_line, "tc", 2) == 0) {
        //    test_tcp_client();
        //    continue;
        //}

        do_cmd(cmd_line);
    }

    exit(0);
    return 1;
}

