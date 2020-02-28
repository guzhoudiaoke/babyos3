/*
 *	babyos/kernel/process.cc
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



#include "process.h"
#include "x86.h"
#include "babyos.h"
#include "string.h"
#include "elf.h"
#include "kernel.h"


extern void ret_from_fork(void) __asm__("ret_from_fork");


process_t* process_t::fork(trap_frame_t* frame)
{
    /* alloc a process_t */
    os()->uart()->kprintf("free page num: %d\n", os()->buddy()->get_free_page_num());

    process_t* p = (process_t *) P2V(os()->buddy()->alloc_pages(1));
    if (p == NULL) {
        os()->console()->kprintf(RED, "fork failed\n");
        return NULL;
    }

    *p = *this;
    p->m_kstack = (uint8 *)p + PAGE_SIZE*2;

    /* frame */
    fork_frame_t* child_frame = ((fork_frame_t *) ((uint64(p) + PAGE_SIZE*2))) - 1;
    memcpy(&child_frame->trap_frame, frame, sizeof(trap_frame_t));
    child_frame->trap_frame.rax = 0;

    /* vmm */
    p->m_vmm.copy(m_vmm);

    /* signal */
    //p->m_signal.copy(m_signal);
    //m_sig_queue.init(os()->get_obj_pool_of_size());
    //m_sig_mask_lock.init();

    /* file */
    p->m_cwd = os()->fs()->dup_inode(m_cwd);
    for (int i = 0; i < MAX_OPEN_FILE; i++) {
        if (m_files[i] != NULL && m_files[i]->m_type != file_t::TYPE_NONE) {
            p->m_files[i] = os()->fs()->dup_file(m_files[i]);
        }
    }

    /* context */
    p->m_context.rsp = (uint64) child_frame;
    child_frame->ret_addr = (uint64) ret_from_fork;

    if ((frame->cs & 0x3) == 0) {
        child_frame->trap_frame.rsp = (uint64(p) + PAGE_SIZE*2);
    }

    /* pid, need check if same with other process */
    p->m_pid = os()->process_mgr()->get_next_pid();

    /* change state */
    p->m_state = RUNNING;
    p->m_need_resched = 0;
    p->m_timeslice = 2;

    p->m_children.init(os()->get_obj_pool_of_size());
    p->m_wait_child.init();
    p->m_parent = this;
    p->m_has_cpu = 0;


    /* link to run queue */
    os()->process_mgr()->add_process_to_rq(p);

    /* add to process list */
    os()->process_mgr()->add_process_to_list(p);

    /* add a child for current */
    m_children.push_back(p);

    return p;
}

int32 process_t::init_arguments(trap_frame_t* frame, argument_t* arg)
{
    if (arg == NULL) {
        frame->rdi = 0;
        frame->rsi = 0;
        return 0;
    }

    /* space for args */
    for (uint32 i = 0; i < arg->m_argc; i++) {
        frame->rsp -= (strlen(arg->m_argv[i]) + 1);
    }
    frame->rsp -= frame->rsp % 8;
    frame->rsp -= arg->m_argc * sizeof(char**); // argv
    frame->rsp -= sizeof(uint64);               // ret address

    /* ret addr */
    uint64 top = frame->rsp;
    *(uint64 *)top = 0xffffffffffffffff;
    top += sizeof(uint64);

    /* argc */
    frame->rdi = arg->m_argc;

    /* argv */
    char** argv = (char **) top;
    frame->rsi = (uint64) argv;

    char* p = (char *) top + sizeof(char *) * arg->m_argc;
    for (uint32 i = 0; i < arg->m_argc; i++) {
        argv[i] = p;
        strcpy(p, arg->m_argv[i]);
        p += (strlen(p) + 1);
    }

    return 0;
}

int32 process_t::init_user_stack(trap_frame_t* frame, argument_t* arg)
{
    /* alloc vma */
    vm_area_t* vma = (vm_area_t *) os()->get_obj_pool(VMA_POOL)->alloc_from_pool();
    if (vma == NULL) {
        os()->console()->kprintf(RED, "BUG on alloc vm_area_t!\n");
        return -1;
    }

    /* insert vma */
    vma->m_end   = USER_STACK_TOP;
    vma->m_start = USER_STACK_TOP - PAGE_SIZE;
    vma->m_page_prot = PROT_READ | PROT_WRITE;
    vma->m_flags = VM_STACK;
    vma->m_next = NULL;
    if (current->m_vmm.insert_vma(vma) != 0) {
        os()->console()->kprintf(RED, "BUG on insert vma!\n");
        return -1;
    }

    /* set user rsp */
    frame->rsp = USER_STACK_TOP - 8;

    /* init args */
    init_arguments(frame, arg);

    return 0;
}

int32 process_t::exec(trap_frame_t* frame)
{
    /* copy process name */
    const char* path = (const char *) frame->rdi;
    strcpy(m_name, path);

    /* save arg */
    argument_t* arg = NULL;
    if (frame->rsi != 0) {
        arg = (argument_t *) P2V(os()->buddy()->alloc_pages(0));
        memcpy(arg, (void *)frame->rsi, sizeof(argument_t));
    }

    /* flush old mmap */
    current->m_vmm.release();

    /* load elf binary */
    if (elf_t::load(frame, m_name) != 0) {
        /* free arg */
        if (arg != NULL) {
            os()->buddy()->free_pages(V2P(arg), 0);
        }
        exit();
        return -1;
    }

    /* code segment, data segment and so on */
    frame->cs = (SEG_UCODE << 3 | 0x3);
    frame->ss = (SEG_UDATA << 3 | 0x3);
    frame->rflags = 0x200;

    /* stack, rsp */
    init_user_stack(frame, arg);

    /* free arg */
    if (arg != NULL) {
        os()->buddy()->free_pages(V2P(arg), 0);
    }

    return 0;
}

static void process_timeout(uint64 data)
{
    process_t* proc = (process_t *) data;
    os()->process_mgr()->wake_up_process(proc);
}

void process_t::sleep(uint64 ticks)
{
    /* add a timer to alarm after ticks */
    timer_t timer;
    timer.init(ticks, (uint64) current, process_timeout);
    os()->timer_mgr()->add_timer(&timer);

    current->m_state = process_t::SLEEP;

    os()->cpu()->schedule();

    /* remove the timer */
    os()->timer_mgr()->remove_timer(&timer);
}

void process_t::sleep_on(wait_queue_t* queue)
{
    queue->add(current);
    current->m_state = process_t::SLEEP;
    os()->cpu()->schedule();
    queue->remove(current);
}

void process_t::set_state(state_t state)
{
    m_state = state;
}

void process_t::adope_children()
{
    process_t* child_reaper = os()->process_mgr()->get_child_reaper();
    list_t<process_t *>::iterator it = m_children.begin();
    while (it != m_children.end()) {
        (*it)->m_parent = child_reaper;
        it++;
    }
}

void process_t::notify_parent()
{
    // SIGCHLD is needed, but now not support signal
    // so just wake up parent
    m_parent->m_wait_child.wake_up();
}

int32 process_t::wait_children(pid_t pid)
{
    current->m_wait_child.add(current);

repeat:
    m_state = process_t::SLEEP;
    list_t<process_t *>::iterator it = m_children.begin();

    bool flag = false;
    for (; it != m_children.end(); it++) {
        process_t* p = *it;
        if (pid != -1u && pid != p->m_pid) {
            continue;
        }

        /* find pid, or find any child if pid == -1 */
        flag = true;

        if (p->m_state != process_t::ZOMBIE) {
            continue;
        }

        /* this child has become ZOMBIE, free it */
        os()->process_mgr()->release_process(p);

        goto end_wait;
    }

    /* if find pid, or any child if pid == -1 */
    if (flag) {
        /* sleep to wait child exit */
        os()->cpu()->schedule();

        /* wake up by a exited child, repead to check */
        goto repeat;
    }

end_wait:
    /* continue to run */
    m_state = process_t::RUNNING;
    current->m_wait_child.remove(current);

    return 0;
}

void process_t::close_all_files()
{
    os()->fs()->put_inode(m_cwd);
    for (int i = 0; i < MAX_OPEN_FILE; i++) {
        if (m_files[i] != NULL && m_files[i]->m_type != file_t::TYPE_NONE) {
            os()->fs()->close_file(m_files[i]);
        }
    }
}

int32 process_t::exit()
{
    /* remove the mem resource */
    m_vmm.release();

    /* close all opend files */
    close_all_files();

    /* adope children to init */
    adope_children();

    /* set state as sleep */
    m_state = process_t::ZOMBIE;

    /* let parent wake up, and mourn us */
    notify_parent();

    /* schedule other process */
    os()->cpu()->schedule();

    return 0;
}

//void process_t::calc_sig_pending()
//{
//    m_sig_pending = !m_sig_queue.empty();
//}

int process_t::alloc_fd(file_t* file)
{
    for (int i = 0; i < MAX_OPEN_FILE; i++) {
        if (m_files[i] == NULL) {
            m_files[i] = file;
            return i;
        }
    }
    return -1;
}

file_t* process_t::get_file(int fd)
{
    if (fd < 0 || fd >= MAX_OPEN_FILE) {
        return NULL;
    }
    return m_files[fd];
}

void process_t::free_fd(int fd)
{
    if (fd >= 0 && fd < MAX_OPEN_FILE) {
        current->m_files[fd] = NULL;
    }
}

void process_t::set_cwd(inode_t* inode)
{
    m_cwd = inode;
}

//extern "C"
//void do_signal(trap_frame_t* frame)
//{
//    current->do_signal(frame);
//}
//
//void process_t::do_signal(trap_frame_t* frame)
//{
//    if (!m_sig_queue.empty()) {
//        siginfo_t si = *current->m_sig_queue.begin();
//        m_sig_queue.pop_front();
//        calc_sig_pending();
//        m_signal.handle_signal(frame, si);
//    }
//}

void process_t::lock()
{
    m_task_lock.lock();
}

void process_t::unlock()
{
    m_task_lock.unlock();
}

