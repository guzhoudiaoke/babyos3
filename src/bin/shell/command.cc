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

command_t::command_t()
{
}

command_t::~command_t()
{
}

char* redirect_file_t::name()
{
    return m_name;
}

exec_command_t::exec_command_t()
{
    m_input = NULL;
    m_output = NULL;
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

void exec_command_t::exec()
{
    printf("cmd: %s, input: %s, output: %s\n", m_argument.m_argv[0], 
            m_input == NULL ? "null" : m_input->name(),
            m_output == NULL ? "null" : m_output->name());

    printf("args(%d): ", m_argument.m_argc-1);
    for (uint32 i = 1; i < m_argument.m_argc; i++) {
        printf("%s, ", m_argument.m_argv[i]);
    }
    printf("\nback: %d\n", m_background);
}


pipe_command_t::pipe_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

pipe_command_t::~pipe_command_t()
{
}

void pipe_command_t::exec()
{
    if (m_left != NULL) {
        m_left->exec();
    }
    if (m_right != NULL) {
        m_right->exec();
    }
}

list_command_t::list_command_t(command_t* left, command_t* right)
{
    m_left = left;
    m_right = right;
}

list_command_t::~list_command_t()
{
}


void list_command_t::exec()
{
    if (m_left != NULL) {
        m_left->exec();
    }
    if (m_right != NULL) {
        m_right->exec();
    }
}

