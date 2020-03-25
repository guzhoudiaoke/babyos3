/*
 *	babyos/user/rm.cc
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
    if (argc < 2) {
        printf("Usage: rm files...\n");
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (unlink(argv[i]) < 0) {
            printf("rm %s failed\n", argv[i]);
            break;
        }
    }

    return 0;
}


