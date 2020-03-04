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




#include "userlib.h"
#include "file.h"

int main()
{
    if (userlib_t::open("/dev/console", file_t::MODE_RDWR) < 0) {
        userlib_t::mkdir("/dev/");
        userlib_t::mknod("/dev/console", 0, 1);
        userlib_t::open("/dev/console", file_t::MODE_RDWR);
    }
    userlib_t::dup(0); // stdout
    userlib_t::dup(0); // stderr


    // print cs to show work in user mode
    uint32 cs = 0xffffffff;
    __asm__ volatile("movl %%cs, %%eax" : "=a" (cs));
    userlib_t::printf("This is printed by init, cs = 0x%8x\n", cs);


    int32 pid = userlib_t::fork();
    userlib_t::printf("fork done, pid = %d, ", pid);
    if (pid == 0) {
        userlib_t::printf("this is child, will do exec\n");

        /* child */
        int ret = userlib_t::exec("/bin/shell", NULL);
        if (ret != 0) {
            userlib_t::printf("BUG exec failed!!!\n");
            userlib_t::exit(-1);
        }
    }

    //userlib_t::printf("this is parent, will do dead loop\n");
    //while (1) {
    //    userlib_t::loop_delay(300000000);
    //    userlib_t::printf("I,");
    //}

    while (1) {
        userlib_t::wait(-1);
    }

#if 0
    // fork
    int32 pid = userlib_t::fork();
    if (pid == 0) {
        // child
        pid = userlib_t::exec(SHELL_LBA, SHELL_SECT_NUM, "shell");
        if (pid != 0) {
            userlib_t::print("BUG exec failed!!!\n");
        }
        while (1) {
            userlib_t::loop_delay(100000000);
            userlib_t::print("IC,");
        }
    }
    else {
        userlib_t::wait(pid);

        // parent
        while (1) {
            userlib_t::sleep(2);
            userlib_t::print("I,");
            userlib_t::kill(pid, 4);
        }
    }
#endif

    return 0;
}

