/*
 *	babyos/bin/test_sig.cc
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
#include "signal.h"


void sig_handler(int sig)
{
    printf("Receive signal: %d\n", sig);
}

int main()
{
    int i = 0;
    pid_t pid = fork();

    if (pid == 0) {
        /* child */
        sighandler_t handler = sig_handler;
        signal(4, handler);
        while (i++ < 10) {
            sleep(2);
        }
        exit(0);
    }

    while (i++ < 10) {
        sleep(2);
        kill(pid, 4);
        printf("Send signal\n");
    }

    wait(pid);
    return 0;
}