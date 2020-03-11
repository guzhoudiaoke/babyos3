/*
 *	babyos/src/bin/shell/shell.cc
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




#ifndef _SHELL_H_
#define _SHELL_H_


#include "arg.h"
#include "command.h"
#include "parser.h"


#define MAX_LINE    1024


class shell_t {
public:
    shell_t();
    ~shell_t();

    void run();

private:
    void process(char* cmdline);

private:
    argument_t m_argument;
};

#endif
