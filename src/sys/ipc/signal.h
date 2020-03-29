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


#define SIGINVAL 0
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPWR 30
#define SIGSYS 31
#define NSIG 32

#define SIG_DFL ((sighandler_t)0)
#define SIG_ERR ((sighandler_t)-1)
#define SIG_IGN ((sighandler_t)1)


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