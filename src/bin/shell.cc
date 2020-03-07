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
#include "malloc.h"


shell_t::shell_t()
{
}
shell_t::~shell_t()
{
}


const char* shell_t::parse_cmd(const char* cmd_line, char* cmd)
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

void shell_t::parse_cmd_line(const char* cmd_line, char* cmd, argument_t* arg)
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

void shell_t::do_cmd(const char* cmd_line)
{
    parse_cmd_line(cmd_line, m_command, &m_argument);
    int32 pid = fork();
    if (pid == 0) {
        int ret = exec(m_command, &m_argument);
        printf("ERROR: shell exec returned: %d\n", pid);
        if (ret < 0) {
            exit(-1);
        }
    }

    wait(pid);
}

void shell_t::test_fork_exec_wait_exit(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    m_argument.m_argc = 1;
    m_argument.m_argv[0][0] = 0;
    for (int i = 0; i < t; i++) {
        int32 pid = fork();
        if (pid == 0) {
            int ret = exec("/bin/ls", &m_argument);
            if (ret < 0) {
                exit(-1);
            }
            exit(0);
        }

        wait(pid);
    }
}

// only for test
void shell_t::test_fork_wait_exit(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    m_argument.m_argc = 1;
    m_argument.m_argv[0][0] = 0;
    for (int i = 0; i < t; i++) {
        int32 pid = fork();
        if (pid == 0) {
                printf("%u\n", i);
            exit(0);
        }

        wait(pid);
    }
}

void shell_t::test_pipe(const char* times)
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


void shell_t::run()
{
    char* cmd_line = (char *) malloc(MAX_CMD_LEN);
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

        do_cmd(cmd_line);
    }

    free(cmd_line);
}

int main()
{
    shell_t shell;
    shell.run();

    exit(0);
    return 1;
}

