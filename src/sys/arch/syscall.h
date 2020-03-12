/*
 *	babyos/kernel/syscall.h
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
 *  2020-02-16		created
 */


#ifndef _SYSCALL_H_
#define _SYSCALL_H_


#include "types.h"
#include "traps.h"
#include "syscall_def.h"


class syscall_t {
public:
    static void   init();
    static void   do_syscall(trap_frame_t* frame);
    static uint64 get_argument(trap_frame_t* frame, uint32 index);

    static uint64 sys_print(trap_frame_t* frame);
    static uint64 sys_fork(trap_frame_t* frame);
    static uint64 sys_exec(trap_frame_t* frame);
    static uint64 sys_mmap(trap_frame_t* frame);
    static uint64 sys_exit(trap_frame_t* frame);
    static uint64 sys_wait(trap_frame_t* frame);
    static uint64 sys_sleep(trap_frame_t* frame);
    static uint64 sys_open(trap_frame_t* frame);
    static uint64 sys_close(trap_frame_t* frame);
    static uint64 sys_read(trap_frame_t* frame);
    static uint64 sys_write(trap_frame_t* frame);
    static uint64 sys_link(trap_frame_t* frame);
    static uint64 sys_unlink(trap_frame_t* frame);
    static uint64 sys_mkdir(trap_frame_t* frame);
    static uint64 sys_mknod(trap_frame_t* frame);
    static uint64 sys_dup(trap_frame_t* frame);
    static uint64 sys_stat(trap_frame_t* frame);
    static uint64 sys_chdir(trap_frame_t* frame);
    static uint64 sys_pipe(trap_frame_t* frame);
    static uint64 sys_send_to(trap_frame_t* frame);
    static uint64 sys_recv_from(trap_frame_t* frame);
    static uint64 sys_sbrk(trap_frame_t* frame);

    //static uint64 sys_signal(trap_frame_t* frame);
    //static uint64 sys_sigret(trap_frame_t* frame);
    //static uint64 sys_kill(trap_frame_t* frame);

private:
    static uint64 (*s_system_call_table[MAX_SYSCALL])(trap_frame_t* frame);
};

#endif
