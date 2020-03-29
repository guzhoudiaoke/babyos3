/*
 *	babyos/bin/ps.cc
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
 *  2020-03-29		created
 */



#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"


const int c_buffer_size = 4096;

int main(int argc, char** argv)
{
    char* buffer = (char *) malloc(c_buffer_size);
    int ret = list_process(buffer, c_buffer_size);
    if (ret == 0) {
        printf("%3s %20s %8s\n", "PID", "CMD", "STATE");
        printf("%s", buffer);
    }

    free(buffer);
    return 0;
}