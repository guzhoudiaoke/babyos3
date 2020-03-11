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



#include "color.h"
#include "shell.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "cxx.h"


shell_t::shell_t()
{
}
shell_t::~shell_t()
{
}


void shell_t::process(char* cmdline)
{
    parser_t parser;
    parser.init(cmdline);

    command_t* cmd = parser.parse();
    cmd->exec();
}

void shell_t::run()
{
    char* cmd_line = (char *) malloc(MAX_CMD_LEN);
    while (true) {
        color_print(CYAN, "liuruyi $ ");
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

        process(cmd_line);
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

