/*
 *	babyos/src/bin/shell/command.cc
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
 *  2020-03-11		created
 */


#include "command.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "cxx.h"


redirect_file_t::redirect_file_t(char* s, char* e)
{
    m_name = new char[e-s+1];
    strncpy(m_name, s, e-s);
}

redirect_file_t::~redirect_file_t()
{
}

char* redirect_file_t::name()
{
    return m_name;
}


/**********************************************************************/


command_t::command_t()
{
}

command_t::~command_t()
{
}

/*******************************************************************************/

exec_command_t::exec_command_t()
{
    m_input = nullptr;
    m_output = nullptr;
    m_background = false;
    m_argument.m_argc = 0;
}

exec_command_t::~exec_command_t()
{
}

void exec_command_t::add_arg(char* s, char* e)
{
    strncpy(m_argument.m_argv[m_argument.m_argc++], s, e-s);
}

void exec_command_t::execute()
{
    char cmd[MAX_CMD_LEN] = {0};
    if (m_argument.m_argv[0][0] != '/') {
        strcpy(cmd, "/bin/");
    }
    strcat(cmd, m_argument.m_argv[0]);

    if (m_input != nullptr) {
        int fd = open(m_input->name(), MODE_RDONLY);
        if (fd < 0) {
            printf("failed to open %s\n", m_input->name());
            return;
        }

        close(0);
        dup(fd);
        close(fd);
    }
    if (m_output != nullptr) {
        int fd = open(m_output->name(), MODE_WRONLY | MODE_CREATE);
        if (fd < 0) {
            printf("failed to open %s\n", m_output->name());
            return;
        }

        close(1);
        dup(fd);
        close(fd);
    }

    exec(cmd, &m_argument);
    printf("exec failed\n");
}

/*******************************************************************************/

pipe_command_t::pipe_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

pipe_command_t::~pipe_command_t()
{
    if (m_left != nullptr) {
        delete m_left;
    }
    if (m_right != nullptr) {
        delete m_right;
    }
}

void pipe_command_t::execute()
{
    int fd[2];
    if (pipe(fd) < 0) {
        printf("pipe failed\n");
        exit(-1);
    }
    
    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) {
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);
        m_left->execute();
    }
    if ((pid2 = fork()) == 0) {
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        m_right->execute();
    }
    close(fd[0]);
    close(fd[1]);

    wait(pid1);
    wait(pid2);
}

/*******************************************************************************/

list_command_t::list_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

list_command_t::~list_command_t()
{
    if (m_left != nullptr) {
        delete m_left;
    }
    if (m_right != nullptr) {
        delete m_right;
    }
}

void list_command_t::execute()
{
    if (m_left != nullptr) {
        m_left->execute();
    }
    if (m_right != nullptr) {
        m_right->execute();
    }
}