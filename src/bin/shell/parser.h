/*
 *	babyos/src/bin/shell/parser.h
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


#ifndef _PARSER_H_
#define _PARSER_H_

#include "command.h"


class token_t {
public:
    token_t(char* line, int begin, int end);
    ~token_t();

    char* m_line;
    int   m_begin;
    int   m_end;
};


class cmd_parser_t {
public:
    cmd_parser_t(char* line, int begin, int end);
    ~cmd_parser_t();

    command_t* parse();

private:
    redirect_file_t* parse_file();
    token_t* get_next_token();

public:
    char *m_line;
    int   m_begin;
    int   m_end;
};


class parser_t {

public:
    parser_t(char* line);
    ~parser_t();

    command_t* parse();

private:
    cmd_parser_t* get_next_command();

private:
    char* m_line;
    int   m_begin;
    int   m_end;
};



#endif
