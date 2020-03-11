/*
 *	babyos/kernel/process_mgr.h
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



#include "process_mgr.h"
#include "babyos.h"
#include "x86.h"
#include "string.h"


void process_mgr_t::init()
{
    atomic_set(&m_next_pid, 1);
    m_child_reaper = nullptr;

    m_run_queue_lock.init();
    m_proc_list_lock.init();

    m_proc_list.init();
    m_run_queue.init();
}

process_t* process_mgr_t::find_process(pid_t pid)
{
    dlist_node_t* node = m_proc_list.head();
    while (node != nullptr) {
        process_t* p = list_entry(node, process_t, m_mgr_list_node);
        if (p->m_pid == pid) {
            return p;
        }
        node = node->next();
    }

    return nullptr;
}

bool process_mgr_t::in_run_queue(process_t* proc)
{
    dlist_node_t* node = m_run_queue.head();
    while (node != nullptr) {
        process_t* p = list_entry(node, process_t, m_rq_list_node);
        if (p == proc) {
            return true;
        }
        node = node->next();
    }

    return false;
}

process_t* process_mgr_t::get_child_reaper()
{
    /* if have not set init process, set it by idle process's child */
    if (m_child_reaper == nullptr) {
        m_child_reaper = list_entry(os()->cpu()->get_idle_process()->m_children.head(),
                                    process_t,
                                    m_child_list_node);
    }

    return m_child_reaper;
}

void process_mgr_t::release_process(process_t* proc)
{
    /* wait to make sure this proc not running on other processor */
    while (1) {
        proc->lock();
        if (!proc->m_has_cpu) {
            break;
        }
        proc->unlock();

        do {
            nop();
            barrier();
        } while (proc->m_has_cpu);
    }
    proc->unlock();

    /* remove from proc list */
    remove_from_process_list(proc);

    /* remove from children of parent */
    if (proc->m_parent != nullptr) {
        proc->m_parent->m_children.remove(&proc->m_child_list_node);
    }

    /* free page dir */
    os()->mm()->free_pages(V2P(proc->m_vmm.get_pml4_table()), 0);

    /* free task */
    os()->mm()->free_pages(V2P(proc), 1);
}

void process_mgr_t::add_process_to_rq(process_t* proc)
{
    uint64 flags;
    m_run_queue_lock.lock_irqsave(flags);
    if (!in_run_queue(proc)) {
        m_run_queue.add_head(&proc->m_rq_list_node);
    }
    m_run_queue_lock.unlock_irqrestore(flags);
}

void process_mgr_t::remove_process_from_rq(process_t* proc)
{
    uint64 flags;
    m_run_queue_lock.lock_irqsave(flags);
    if (!in_run_queue(proc)) {
        os()->panic("removing proc from run queue not in run queue");
    }
    m_run_queue.remove(&proc->m_rq_list_node);
    m_run_queue_lock.unlock_irqrestore(flags);
}

spinlock_t* process_mgr_t::get_rq_lock()
{
    return &m_run_queue_lock;
}

spinlock_t* process_mgr_t::get_proc_list_lock()
{
    return &m_proc_list_lock;
}

void process_mgr_t::add_process_to_list(process_t* proc)
{
    uint64 flags;
    m_proc_list_lock.lock_irqsave(flags);
    m_proc_list.add_tail(&proc->m_mgr_list_node);
    m_proc_list_lock.unlock_irqrestore(flags);
}

void process_mgr_t::remove_from_process_list(process_t* proc)
{
    uint64 flags;
    m_proc_list_lock.lock_irqsave(flags);
    m_proc_list.remove(&proc->m_mgr_list_node);
    m_proc_list_lock.unlock_irqrestore(flags);
}

void process_mgr_t::wake_up_process(process_t* proc)
{
    proc->set_state(process_t::RUNNING);
    add_process_to_rq(proc);
}

dlist_t* process_mgr_t::get_run_queue()
{
    return &m_run_queue;
}

uint32 process_mgr_t::get_next_pid()
{
    uint32 pid = 0;
    while (1) {
        pid = atomic_read(&m_next_pid);
        atomic_inc(&m_next_pid);
        if (find_process(pid) == nullptr) {
            break;
        }
    }
    return pid;
}


//int32 process_mgr_t::send_signal_to(uint32 pid, uint32 sig)
//{
//    siginfo_t si;
//    si.m_sig = sig;
//    si.m_pid = current->m_pid;
//
//    spinlock_t* lock = os()->get_process_mgr()->get_proc_list_lock();
//    uint32 flags;
//    lock->lock_irqsave(flags);
//    process_t* p = os()->get_process_mgr()->find_process(pid);
//    if (p != nullptr) {
//        p->m_sig_queue.push_back(si);
//        p->calc_sig_pending();
//    }
//    lock->unlock_irqrestore(flags);
//
//    return 0;
//}
