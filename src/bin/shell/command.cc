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

    pid_t pid = fork();
    if (pid == 0) {
        int ret = exec(cmd, &m_argument);
        if (ret < 0) {
            exit(ret);
        }
    }
    wait(pid);
}


pipe_command_t::pipe_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

pipe_command_t::~pipe_command_t()
{
    printf("pipe_command_t destruct\n");
    if (m_left != nullptr) {
        delete m_left;
    }
    if (m_right != nullptr) {
        delete m_right;
    }
}

void pipe_command_t::execute()
{
    if (m_left != nullptr) {
        m_left->execute();
    }
    if (m_right != nullptr) {
        m_right->execute();
    }
}

list_command_t::list_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

list_command_t::~list_command_t()
{
    printf("list_command_t destruct\n");
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

