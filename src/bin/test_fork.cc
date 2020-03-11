/*
 *	babyos/user/test_fork.cc
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



#include "file.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"


void test_fork_wait_exit(const char* times)
{
    int t = 0;
    while (*times != '\0') {
        if (*times < '0' || *times > '9') {
            break;
        }
        t = t*10 + *times - '0';
        times++;
    }

    for (int i = 0; i < t; i++) {
        int32 pid = fork();
        if (pid == 0) {
            printf("%u\n", i);
            exit(0);
        }

        wait(pid);
    }
}


int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: test_fork times\n");
        exit(-1);
    }

    test_fork_wait_exit(argv[1]);

    exit(0);
    return 0;
}