/*
 *	babyos/src/bin/shell/command.h
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


#ifndef _COMMAND_H_
#define _COMMAND_H_


#include "arg.h"
#include "types.h"


#define MAX_CMD_LEN 128


class redirect_file_t {
public:
    redirect_file_t(char* s, char* e);
    ~redirect_file_t();

    char* name();

private:
    char* m_name;
};


class command_t {
public:
    command_t();
    virtual ~command_t();

    virtual void execute() = 0;
};

class exec_command_t : public command_t {
public:
    exec_command_t();
    ~exec_command_t();

    virtual void execute();
    void add_arg(char* s, char* e);

public:
    argument_t       m_argument;
    redirect_file_t* m_input;
    redirect_file_t* m_output;
    bool             m_background;
};

class pipe_command_t : public command_t {
public:
    pipe_command_t(command_t* left, command_t* right);
    ~pipe_command_t();

    virtual void execute();

public:
    command_t* m_left;
    command_t* m_right;
};

class list_command_t : public command_t {
public:
    list_command_t(command_t* left, command_t* right);
    ~list_command_t();

    virtual void execute();

public:
    command_t* m_left;
    command_t* m_right;
};

#endif
