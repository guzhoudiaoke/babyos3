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
#include "string.h"
#include "x86.h"


int32 (*syscall_t::s_system_call_table[])(trap_frame_t* frame) = {
    syscall_t::sys_print,
    syscall_t::sys_fork,
    syscall_t::sys_exec,
    syscall_t::sys_mmap,
    syscall_t::sys_exit,
    syscall_t::sys_wait,
    syscall_t::sys_sleep,
    //syscall_t::sys_signal,
    //syscall_t::sys_sigret,
    //syscall_t::sys_kill,
    syscall_t::sys_open,
    syscall_t::sys_close,
    syscall_t::sys_read,
    syscall_t::sys_write,
    syscall_t::sys_link,
    syscall_t::sys_unlink,
    syscall_t::sys_mkdir,
    syscall_t::sys_mknod,
    syscall_t::sys_dup,
    syscall_t::sys_stat,
    syscall_t::sys_chdir,
    //syscall_t::sys_pipe,
    //syscall_t::sys_send_to,
    //syscall_t::sys_recv_from,
    //syscall_t::sys_socket,
};

void syscall_t::do_syscall(trap_frame_t* frame)
{
    uint64 id = frame->rax;
    if (id >= MAX_SYSCALL) {
        //os()->console()->kprintf(RED, "unknown system call %x, current: %p\n", id, current->m_pid);
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

int32 syscall_t::sys_print(trap_frame_t* frame)
{
    color_ref_t color = (color_ref_t) get_argument(frame, 0);
    char buffer[512] = {0};
    char* va = (char *)PA2VA(vmm_t::va_to_pa(current->m_vmm.get_pml4_table(), (void *)get_argument(frame, 1)));

    strcpy(buffer, va);
    os()->console()->kprintf(color, "%s", buffer);

    return 0;
}

int32 syscall_t::sys_fork(trap_frame_t* frame)
{
    process_t* proc = current->fork(frame);
    return proc == NULL ? -1 : proc->m_pid;
}

int32 syscall_t::sys_exec(trap_frame_t* frame)
{
    return current->exec(frame);
}

int32 syscall_t::sys_mmap(trap_frame_t* frame)
{
    uint64 addr = get_argument(frame, 0), len = get_argument(frame, 1),
        prot = get_argument(frame, 2), flags = get_argument(frame, 3);
    addr = current->m_vmm.do_mmap(addr, len, prot, flags);
    return addr;
}

int32 syscall_t::sys_exit(trap_frame_t* frame)
{
    return current->exit();
}

int32 syscall_t::sys_wait(trap_frame_t* frame)
{
    pid_t pid = get_argument(frame, 0);
    return current->wait_children(pid);
}

int32 syscall_t::sys_sleep(trap_frame_t* frame)
{
    uint32 ticks = get_argument(frame, 0) * HZ;
    current->sleep(ticks);
    return 0;
}

//int32 syscall_t::sys_signal(trap_frame_t* frame)
//{
//    uint32 sig = frame->ebx;
//    sighandler_t sig_handler = (sighandler_t) frame->ecx;
//    return current->m_signal.do_sigaction(sig, sig_handler);
//}
//
//int32 syscall_t::sys_sigret(trap_frame_t* frame)
//{
//    return current->m_signal.do_sigreturn(frame);
//}
//
//int32 syscall_t::sys_kill(trap_frame_t* frame)
//{
//    uint32 pid = frame->ebx;
//    uint32 sig = frame->ecx;
//    return os()->get_process_mgr()->send_signal_to(pid, sig);
//}
//
int32 syscall_t::sys_open(trap_frame_t* frame)
{
    const char* path = (const char *) get_argument(frame, 0);
    int32 mode = (int32) get_argument(frame, 1);
    return os()->fs()->do_open(path, mode);
}

int32 syscall_t::sys_close(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    return os()->fs()->do_close(fd);
}

int32 syscall_t::sys_read(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    char* buf = (char *) get_argument(frame, 1);
    uint64 size = get_argument(frame, 2);
    return os()->fs()->do_read(fd, buf, size);
}

int32 syscall_t::sys_write(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    char* buf = (char *) get_argument(frame, 1);
    uint64 size = get_argument(frame, 2);
    return os()->fs()->do_write(fd, buf, size);
}

int32 syscall_t::sys_link(trap_frame_t* frame)
{
    char* path_old = (char *) get_argument(frame, 0);
    char* path_new = (char *) get_argument(frame, 1);
    return os()->fs()->do_link(path_old, path_new);
}

int32 syscall_t::sys_unlink(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    return os()->fs()->do_unlink(path);
}

int32 syscall_t::sys_mkdir(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    return os()->fs()->do_mkdir(path);
}

int32 syscall_t::sys_mknod(trap_frame_t* frame)
{
    char* path = (char *) get_argument(frame, 0);
    int major = get_argument(frame, 1);
    int minor = get_argument(frame, 2);
    return os()->fs()->do_mknod(path, major, minor);
}

int32 syscall_t::sys_dup(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    return os()->fs()->do_dup(fd);
}

int32 syscall_t::sys_stat(trap_frame_t* frame)
{
    int fd = (int) get_argument(frame, 0);
    stat_t* st = (stat_t *) get_argument(frame, 1);
    return os()->fs()->do_stat(fd, st);
}

int32 syscall_t::sys_chdir(trap_frame_t* frame)
{
    const char* path = (const char *) get_argument(frame, 0);
    return os()->fs()->do_chdir(path);
}

//int32 syscall_t::sys_pipe(trap_frame_t* frame)
//{
//    int* fd = (int *) frame->ebx;
//    return os()->get_fs()->do_pipe(fd);
//}
//
//int32 syscall_t::sys_socket(trap_frame_t* frame)
//{
//    return sys_socket_t::do_sys_socket(frame);
//}

//int32 syscall_t::sys_send_to(trap_frame_t* frame)
//{
//    int32 fd            = frame->ebx;
//    char* buf           = (char *) frame->ecx;
//    uint32 size         = frame->edx;
//    sock_addr_t* addr   = (sock_addr_t *) frame->esi;
//
//    return os()->get_fs()->do_send_to(fd, buf, size, addr);
//}
//
//int32 syscall_t::sys_recv_from(trap_frame_t* frame)
//{
//    int32 fd            = frame->ebx;
//    char* buf           = (char *) frame->ecx;
//    uint32 size         = frame->edx;
//    sock_addr_t* addr   = (sock_addr_t *) frame->esi;
//    
//    return os()->get_fs()->do_recv_from(fd, buf, size, addr);
//}

