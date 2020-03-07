/*
 *	babyos/user/cp.cc
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



#include "unistd.h"
#include "stdio.h"


int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("Usage: cp from to \n");
        exit(0);
    }

    int fd = open(argv[1], file_t::MODE_RDONLY);
    if (fd < 0) {
        printf("can't open %s\n", argv[1]);
        exit(0);
    }

    int fd2 = open(argv[2], file_t::MODE_CREATE | file_t::MODE_WRONLY);
    if (fd2 < 0) {
        printf("can't create %s\n", argv[2]);
        exit(0);
    }

    char buf[512];

    int n = 0;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, n) != n) {
            printf("cat: write error\n");
            exit(-1);
        }
    }

    close(fd);
    close(fd2);

    exit(0);
    return 0;
}

