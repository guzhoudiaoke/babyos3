/*
 *	babyos/kernel/process.h
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
 *  2020-02-18		created
 */


#ifndef _PROCESS_H_
#define _PROCESS_H_


#include "babyos/types.h"
#include "babyos/arg.h"
#include "dlist.h"
#include "traps.h"
#include "waitqueue.h"
#include "vm.h"
#include "fs.h"
#include "fd.h"
#include "ipc/signal.h"


#define MAX_OPEN_FILE 64


/* get current by kernel stack */
class process_t;
static inline process_t* get_current(void)
{
    process_t* current;
    __asm__("andq %%rsp,%0; ":"=r" (current) : "0" (~8191UL));

    return current;
}
#define current get_current()


typedef struct context_s {
    uint64	r15;
    uint64	r14;
    uint64	r13;
    uint64	r12;
    uint64	r11;
    uint64	rbx;
    uint64	rbp;
    uint64  rsp;
} context_t;



class process_t {
public:
    /* state of process */
    typedef enum state_e {
        RUNNING = 0,
        SLEEP,
        ZOMBIE,
    } state_t;

public:
    void        init(process_t* parent);
    process_t*  fork(trap_frame_t* frame);
    int32       exec(trap_frame_t* frame);
    void        sleep(uint64 ticks);
    void        sleep_on(wait_queue_t* queue);
    void        set_state(state_t state);
    int32       exit();
    int32       wait_children(pid_t pid);
    void        calc_sig_pending();

    int         alloc_fd(file_descriptor_t* file);
    void        free_fd(int fd);
    file_descriptor_t*     get_file(int fd);
    void        set_cwd(inode_t* inode);
    void        do_signal(trap_frame_t* frame);

    void        lock();
    void        unlock();

private:
    int32       init_arguments(trap_frame_t* frame, argument_t* arg);
    int32       init_user_stack(trap_frame_t* frame, argument_t* arg);
    void        notify_parent();
    void        adope_children();
    void        close_all_files();

    void        copy_context(const process_t& rhs, trap_frame_t* frame);
    void        copy_mm(const process_t& rhs);
    void        copy_files(const process_t& rhs);
    void        copy_signal(const process_t& rhs);

public:
    uint32              m_need_resched;
    uint32              m_sig_pending;
    char		        m_name[32];
    pid_t		        m_pid;
    state_t		        m_state;
    uint8*              m_kstack;
    context_t	        m_context;
    vmm_t		        m_vmm;
    uint32              m_timeslice;
    uint32              m_has_cpu;
    spinlock_t          m_task_lock;

    process_t*          m_parent;
    dlist_t             m_children;
    dlist_node_t        m_child_list_node;
    dlist_node_t        m_mgr_list_node;
    dlist_node_t        m_rq_list_node;
    dlist_node_t        m_wq_list_node;

    wait_queue_t        m_wait_child;

    signal_t            m_signal;
    dlist_t             m_sig_queue;
    sigset_t            m_sig_blocked;
    spinlock_t          m_sig_mask_lock;

    inode_t*            m_cwd;
    file_descriptor_t*  m_files[MAX_OPEN_FILE];
};

#endif

