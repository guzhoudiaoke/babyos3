/*
 *	babyos/user/init.cc
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
#include "file.h"
#include "stdio.h"


int main()
{
    if (open("/dev/console", file_t::MODE_RDWR) < 0) {
        mkdir("/dev/");
        mknod("/dev/console", 0, 1);
        open("/dev/console", file_t::MODE_RDWR);
    }
    dup(0); /* stdout */
    dup(0); /* stderr */


    /* print cs to show work in user mode */
    //uint32 cs = 0xffffffff;
    //__asm__ volatile("movl %%cs, %%eax" : "=a" (cs));
    //printf("This is printed by init, cs = 0x%8x\n", cs);


    int32 pid = fork();
    //printf("fork done, pid = %d, ", pid);
    if (pid == 0) {
        //printf("this is child, will do exec\n");

        /* child */
        int ret = exec("/bin/shell", nullptr);
        if (ret != 0) {
            printf("BUG exec failed!!!\n");
            exit(-1);
        }
    }

    while (1) {
        wait(-1);
    }

    return 0;
}

