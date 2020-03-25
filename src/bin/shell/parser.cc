/*
 *	babyos/src/bin/shell/parser.cc
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

#include "parser.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "cxx.h"


static const char* cmd_delimeter = "|;\0";
static const char* token_delimeter = " \t\r\n\v<>&\0";
static const char* whitespace = " \t\r\n\v";


static bool is_space(char ch)
{
    return strchr(whitespace, ch) != nullptr;
}

static bool is_delimeter(char ch, const char* delim)
{
    return strchr(delim, ch) != nullptr;
}

static int skip_space(char* line, int begin, int end)
{
    while (begin < end && is_space(line[begin])) {
        begin++;
    }
    return begin;
}

static int next_delimeter(char* line, int begin, int end, const char* delim)
{
    while (begin < end && !is_delimeter(line[begin], delim)) {
        begin++;
    }
    return begin;
}


/****************************************************************************/

range_t::range_t(int begin, int end)
{
    m_begin = begin;
    m_end = end;
}

range_t::~range_t()
{
}


cmd_parser_t::cmd_parser_t(char* line, int begin, int end)
{
    m_line  = line;
    m_begin = begin;
    m_end   = end;
}

cmd_parser_t::~cmd_parser_t()
{
}

range_t cmd_parser_t::get_next_token()
{
    m_begin = skip_space(m_line, m_begin, m_end);

    int begin = m_begin, end;
    if (is_delimeter(m_line[m_begin], token_delimeter)) {
        end = begin+1;
    }
    else {
        end = next_delimeter(m_line, m_begin, m_end, token_delimeter);
    }

    m_begin = end;
    return range_t(begin, end);
}

redirect_file_t* cmd_parser_t::parse_file()
{
    range_t next = get_next_token();
    redirect_file_t *file = new redirect_file_t(&m_line[next.m_begin], &m_line[next.m_end]);
    return file;
}

command_t* cmd_parser_t::parse()
{
    exec_command_t* cmd = new exec_command_t();
    while (m_begin < m_end) {
        range_t token = get_next_token();
        switch (m_line[token.m_begin]) {
        case '<':
            cmd->m_input = parse_file();
            break;
        case '>':
            cmd->m_output = parse_file();
            break;
        case '&':
            cmd->m_background = true;
            m_begin++;
            break;
        default:
            cmd->add_arg(&m_line[token.m_begin], &m_line[token.m_end]);
            break;
        }
        m_begin = skip_space(m_line, m_begin, m_end);
    }
    return cmd;
}


parser_t::parser_t(char* line)
{
    m_begin = 0;
    m_end   = 0;
    m_line  = line;

    if (m_line != nullptr) {
        m_end = strlen(m_line);
    }
}

parser_t::~parser_t()
{
}

command_t* parser_t::parse()
{
    printf("%s, [%d, %d)", m_line, m_begin, m_end);
    if (m_begin >= m_end) {
        return nullptr;
    }

    cmd_parser_t* cmd_parser = get_next_command();
    command_t* cmd = nullptr;
    switch (m_line[cmd_parser->m_end]) {
    case '|':
        cmd = new pipe_command_t(cmd_parser->parse(), parse());
        break;
    case ';':
        cmd = new list_command_t(cmd_parser->parse(), parse());
        break;
    case '\0':
        cmd = cmd_parser->parse();
        break;
    default:
        printf("error");
        break;
    }

    return cmd;
}

cmd_parser_t* parser_t::get_next_command()
{
    int begin, end;

    begin = skip_space(m_line, m_begin, m_end);
    m_begin = begin;

    end = next_delimeter(m_line, m_begin, m_end, cmd_delimeter);
    m_begin = end+1;

    cmd_parser_t* cmd_parser = new cmd_parser_t(m_line, begin, end);
    return cmd_parser;
}

