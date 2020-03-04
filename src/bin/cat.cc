/*
 *	babyos/user/cat.cc
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


void cat(int fd)
{
    char buf[512];

    int n = 0;
    while ((n = userlib_t::read(fd, buf, sizeof(buf))) > 0) {
        if (userlib_t::write(1, buf, n) != n) {
            userlib_t::printf("cat: write error\n");
            userlib_t::exit(-1);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1) {
        cat(0);
        userlib_t::exit(0);
    }

    for (int i = 1; i < argc; i++) {
        int fd = userlib_t::open(argv[i], file_t::MODE_RDONLY);
        if (fd < 0) {
            userlib_t::printf("cat: cannot open %s\n", argv[i]);
            userlib_t::exit(0);
        }

        cat(fd);
        userlib_t::close(fd);
    }

    userlib_t::exit(0);
    return 0;
}

