/*
 *	babyos/sys/ipc/signal.h
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
 *  2020-03-26		created
 */


#ifndef _SIGNAL_H_
#define _SIGNAL_H_


#include "types.h"
#include "traps.h"
#include "spinlock.h"
#include "atomic.h"
#include "dlist.h"



typedef void (*sighandler_t) (int32);
typedef uint64 sigset_t;

#define NSIG    32
#define SIG_DFL  (sighandler_t)(-1)

enum sig_no_e {
    SIG_KILL = 0,
    SIG_SEGV,
};


typedef struct sigaction_s {
    sighandler_t    m_handler;
    uint64          m_flags;
    sigset_t        m_mask;
} sigaction_t;


typedef struct siginfo_s {
    uint32          m_sig;
    uint32          m_pid;
    dlist_node_t    m_node;
} siginfo_t;


typedef struct sigframe_s {
	char*           m_ret_addr;
    trap_frame_t    m_trap_frame;
	char            m_ret_code[8];
} sigframe_t;


class signal_t {
public:
    signal_t();
    ~signal_t();
    void init();
    void copy(const signal_t& signal);

    void lock();
    void unlock();
    void set_sigaction(uint32 sig, const sigaction_t& sa);
    int32 do_sigaction(uint32 sig, sighandler_t sig_handler);
    int32 handle_signal(trap_frame_t* frame, const siginfo_t* si);
    int32 handle_signal_default(uint32 sig);
    int32 do_sigreturn(trap_frame_t* frame);


public:

private:
    atomic_t    m_count;
    sigaction_t m_action[NSIG];
    spinlock_t  m_lock;
};


#endif