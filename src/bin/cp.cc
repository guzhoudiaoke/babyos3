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



#include "userlib.h"


int main(int argc, char** argv)
{
    if (argc < 3) {
        userlib_t::printf("Usage: cp from to \n");
        userlib_t::exit(0);
    }

    int fd = userlib_t::open(argv[1], file_t::MODE_RDONLY);
    if (fd < 0) {
        userlib_t::printf("can't open %s\n", argv[1]);
        userlib_t::exit(0);
    }

    int fd2 = userlib_t::open(argv[2], file_t::MODE_CREATE | file_t::MODE_WRONLY);
    if (fd2 < 0) {
        userlib_t::printf("can't create %s\n", argv[2]);
        userlib_t::exit(0);
    }

    char buf[512];

    int n = 0;
    while ((n = userlib_t::read(fd, buf, sizeof(buf))) > 0) {
        if (userlib_t::write(fd2, buf, n) != n) {
            userlib_t::printf("cat: write error\n");
            userlib_t::exit(-1);
        }
    }

    userlib_t::close(fd);
    userlib_t::close(fd2);

    userlib_t::exit(0);
    return 0;
}

