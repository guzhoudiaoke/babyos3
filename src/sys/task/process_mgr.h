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



#ifndef _PROCESS_MGR_H_
#define _PROCESS_MGR_H_


#include "babyos/types.h"
#include "process.h"
#include "list.h"
#include "spinlock.h"
#include "atomic.h"


class process_mgr_t {
public:
    void        init();
    process_t*  get_child_reaper();
    process_t*  find_process(pid_t pid);
    void        release_process(process_t* proc);
    void        add_process_to_rq(process_t* proc);
    void        remove_process_from_rq(process_t* proc);
    void        add_process_to_list(process_t* proc);
    void        remove_from_process_list(process_t* proc);
    void        wake_up_process(process_t* proc);
    int32       send_signal_to(uint32 pid, uint32 sig);

    spinlock_t* get_rq_lock();
    spinlock_t* get_proc_list_lock();
    dlist_t*    get_run_queue();
    uint32      get_next_pid();
    bool        in_run_queue(process_t* proc);
    int32       list_process(char* buffer, uint32 size);


private:
	atomic_t	        m_next_pid;
    process_t*		    m_child_reaper;

    dlist_t             m_run_queue;
    spinlock_t          m_run_queue_lock;

    dlist_t             m_proc_list;
    spinlock_t          m_proc_list_lock;
};


#endif

