/*
 *	babyos/kernel/syscall.cc
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



#include "kernel.h"
#include "syscall.h"
#include "babyos.h"
#include "kstring.h"
#include "x86.h"
#include "sock_addr.h"
#include "sys_socket.h"
#include "signal.h"


uint64 (*syscall_t::s_system_call_table[])(trap_frame_t* frame);

void syscall_t::init()
{
    s_system_call_table[PRINT]    = syscall_t::sys_print,
    s_system_call_table[FORK]     = syscall_t::sys_fork,
    s_system_call_table[EXEC]     = syscall_t::sys_exec,
    s_system_call_table[MMAP]     = syscall_t::sys_mmap,
    s_system_call_table[EXIT]     = syscall_t::sys_exit,
    s_system_call_table[WAIT]     = syscall_t::sys_wait,
    s_system_call_table[SLEEP]    = syscall_t::sys_sleep,
    s_system_call_table[OPEN]     = syscall_t::sys_open,
    s_system_call_table[CLOSE]    = syscall_t::sys_close,
    s_system_call_table[READ]     = syscall_t::sys_read,
    s_system_call_table[WRITE]    = syscall_t::sys_write,
    s_system_call_table[LINK]     = syscall_t::sys_link,
    s_system_call_table[UNLINK]   = syscall_t::sys_unlink,
    s_system_call_table[MKDIR]    = syscall_t::sys_mkdir,
    s_system_call_table[MKNOD]    = syscall_t::sys_mknod,
    s_system_call_table[DUP]      = syscall_t::sys_dup,
    s_system_call_table[STAT]     = syscall_t::sys_stat,
    s_system_call_table[CHDIR]    = syscall_t::sys_chdir,
    s_system_call_table[PIPE]     = syscall_t::sys_pipe,
    s_system_call_table[SENDTO]   = syscall_t::sys_send_to,
    s_system_call_table[RECVFROM] = syscall_t::sys_recv_from,
    s_system_call_table[SOCKET]   = sys_socket_t::sys_socket;
    s_system_call_table[BIND]     = sys_socket_t::sys_bind;
    s_system_call_table[LISTEN]   = sys_socket_t::sys_listen;
    s_system_call_table[ACCEPT]   = sys_socket_t::sys_accept;
    s_system_call_table[CONNECT]  = sys_socket_t::sys_connect;
    s_system_call_table[SBRK]     = syscall_t::sys_sbrk;
    s_system_call_table[SIGNAL]   = syscall_t::sys_signal;
    s_system_call_table[SIGRET]   = syscall_t::sys_sigret;
    s_system_call_table[KILL]     = syscall_t::sys_kill;
}

void syscall_t::do_syscall(trap_frame_t* frame)
{
    uint64 id = frame->rax;
    if (id >= MAX_SYSCALL) {
        os()->console()->kprintf(RED, "unknown system call %x\n", id);
        frame->rax = -1;
    }
    else {
        frame->rax = s_system_call_table[id](frame);
    }
}

uint64 syscall_t::get_argument(trap_frame_t* frame, uint32 index)
{
    switch (index) {
    case 0:
        return frame->rdi;
    case 1:
        return frame->rsi;
    case 2:
        return frame->rdx;
    case 3:
        return frame->rcx;
    case 4:
        return frame->r8;
    case 5:
        return frame->r9;
    default:
        halt();
    }
    return 0;
}

uint64 syscall_t::sys_print(trap_frame_t* frame)
{
    color_ref_t color = (color_ref_t) get_argument(frame, 0);
    char buffer[512] = {0};
    char* va = (char *)P2V(vmm_t::va_to_pa(current->m_vmm.get_pml4_table(), (void *)get_argument(frame, 1)));

    strcpy(buffer, va);
    os()->console()->kprintf(color, "%s", buffer);

    return 0;
}

uint64 syscall_t::sys_fork(trap_frame_t* frame)
{
    process_t* proc = current->fork(frame);
    return proc == nullptr ? -1 : proc->m_pid;
}

uint64 syscall_t::sys_exec(trap_frame_t* frame)
{
    return current->exec(frame);
}

uint64 syscall_t::sys_mmap(trap_frame_t* frame)
{
    uint64 addr = get_argument(frame, 0), len = get_argument(frame, 1),
        prot = get_argument(frame, 2), flags = get_argument(frame, 3);
    addr = current->m_vmm.do_mmap(addr, len, prot, flags);
    return addr;
}

uint64 syscall_t::sys_exit(trap_frame_t* frame)
{
    return current->exit();
}

uint64 syscall_t::sys_wait(trap_frame_t* frame)
{
    pid_t pid = get_argument(frame, 0);
    return current->wait_children(pid);
}

uint64 syscall_t::sys_sleep(trap_frame_t* frame)
{
    uint32 ticks = get_argument(frame, 0) * HZ;
    current->sleep(ticks);
    return 0;
}

uint64 syscall_t::sys_open(trap_frame_t* frame)
{
    const char* path = (const char *) get_argument(frame, 0);
    int32 mode = (int32) get_argument(frame, 1);
    return os()->fs()->do_open(path, mode);
}

uint64 syscall_t::sys_close(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    return os()->fs()->do_close(fd);
}

uint64 syscall_t::sys_read(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    char* buf = (char *) get_argument(frame, 1);
    uint64 size = get_argument(frame, 2);
    return os()->fs()->do_read(fd, buf, size);
}

uint64 syscall_t::sys_write(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    char* buf = (char *) get_argument(frame, 1);
    uint64 size = get_argument(frame, 2);
    return os()->fs()->do_write(fd, buf, size);
}

uint64 syscall_t::sys_link(trap_frame_t* frame)
{
    char* path_old = (char *) get_argument(frame, 0);
    char* path_new = (char *) get_argument(frame, 1);
    return os()->fs()->do_link(path_old, path_new);
}

uint64 syscall_t::sys_unlink(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    return os()->fs()->do_unlink(path);
}

uint64 syscall_t::sys_mkdir(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    return os()->fs()->do_mkdir(path);
}

uint64 syscall_t::sys_mknod(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    int major = get_argument(frame, 1);
    int minor = get_argument(frame, 2);
    return os()->fs()->do_mknod(path, major, minor);
}

uint64 syscall_t::sys_dup(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    return os()->fs()->do_dup(fd);
}

uint64 syscall_t::sys_stat(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    stat_t* st = (stat_t *) get_argument(frame, 1);
    return os()->fs()->do_stat(fd, st);
}

uint64 syscall_t::sys_chdir(trap_frame_t* frame)
{
    const char* path = (const char *) get_argument(frame, 0);
    return os()->fs()->do_chdir(path);
}

uint64 syscall_t::sys_pipe(trap_frame_t* frame)
{
    int* fd = (int *) get_argument(frame, 0);
    return os()->fs()->do_pipe(fd);
}

uint64 syscall_t::sys_send_to(trap_frame_t* frame)
{
    int32 fd            = (int32) get_argument(frame, 0);
    char* buf           = (char *) get_argument(frame, 1);
    uint64 size         = (uint64) get_argument(frame, 2);
    sock_addr_t* addr   = (sock_addr_t *) get_argument(frame, 2);

    return os()->fs()->do_send_to(fd, buf, size, addr);
}

uint64 syscall_t::sys_recv_from(trap_frame_t* frame)
{
    int32 fd            = (int32) get_argument(frame, 0);
    char* buf           = (char *) get_argument(frame, 1);
    uint64 size         = (uint64) get_argument(frame, 2);
    sock_addr_t* addr   = (sock_addr_t *) get_argument(frame, 2);

    return os()->fs()->do_recv_from(fd, buf, size, addr);
}


uint64 syscall_t::sys_sbrk(trap_frame_t* frame)
{
    uint64 increment = (uint64) get_argument(frame, 0);
    return current->m_vmm.sbrk(increment);
}

uint64 syscall_t::sys_signal(trap_frame_t* frame)
{
    uint32 sig = (uint32) get_argument(frame, 0);
    sighandler_t sig_handler = (sighandler_t) get_argument(frame, 1);
    return current->m_signal.do_sigaction(sig, sig_handler);
}

uint64 syscall_t::sys_sigret(trap_frame_t* frame)
{
    return current->m_signal.do_sigreturn(frame);
}

uint64 syscall_t::sys_kill(trap_frame_t* frame)
{
    uint32 pid = (uint32) get_argument(frame, 0);
    uint32 sig = (uint32) get_argument(frame, 1);
    return os()->process_mgr()->send_signal_to(pid, sig);
}
