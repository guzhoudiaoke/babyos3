/*
 *	babyos/user/mv.cc
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
        printf("Usage: mv from to \n");
        exit(0);
    }

    if (link(argv[1], argv[2]) < 0) {
        printf("mv failed\n");
        exit(0);
    }

    if (unlink(argv[1]) < 0) {
        unlink(argv[2]);
    }

    return 0;
}


