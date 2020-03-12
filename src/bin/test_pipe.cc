/*
 *	babyos/user/test_pipe.cc
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



#include "unistd.h"
#include "stdio.h"
#include "string.h"


void test_pipe(const char* times)
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

        int pid = fork();
        if (pid == 0) {
            close(fd[0]);

            char ch = 'a';
            for (int i = 0; i < 5; i++, ch++) {
                write(fd[1], &ch, 1);
                printf("child write %c to pipe\n", ch);
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


int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: test_pipe times\n");
        exit(-1);
    }

    test_pipe(argv[1]);

    exit(0);
    return 0;
}

