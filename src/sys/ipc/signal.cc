/*
 *	babyos/sys/ipc/signal.cc
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


#include "signal.h"
#include "kstring.h"
#include "process.h"
#include "babyos.h"
#include "syscall_def.h"


signal_t::signal_t()
{
}

signal_t::~signal_t()
{
}

void signal_t::init()
{
    atomic_set(&m_count, 0);
    m_lock.init();

    for (int i = 0; i < NSIG; i++) {
        m_action[i].m_handler = SIG_DFL;
        m_action[i].m_mask = 0;
        m_action[i].m_flags = 0;
    }
}

void signal_t::copy(const signal_t& signal)
{
    atomic_set(&m_count, atomic_read(&signal.m_count));
    memcpy(m_action, signal.m_action, sizeof(sigaction_t) * NSIG);
    m_lock.init();
}

void signal_t::lock()
{
    m_lock.lock();
}

void signal_t::unlock()
{
    m_lock.unlock();
}

void signal_t::set_sigaction(uint32 sig, const sigaction_t& sa)
{
    m_action[sig - 1].m_handler = sa.m_handler;
    m_action[sig - 1].m_flags   = sa.m_flags;
    m_action[sig - 1].m_mask    = sa.m_mask;
}

int32 signal_t::do_sigaction(uint32 sig, sighandler_t sig_handler)
{
    if (sig < 1 || sig >= NSIG) {
        return -1;
    }

    sigaction_t sa;
    sa.m_handler = sig_handler;

    lock();
    set_sigaction(sig, sa);
    unlock();

    return 0;
}

int32 signal_t::handle_signal_default(uint32 sig)
{
    if (sig == SIG_SEGV) {
        os()->uart()->kprintf("handle sigsegv\n");
        current->exit();
    }
    return 0;
}

int32 signal_t::handle_signal(trap_frame_t* frame, const siginfo_t* si)
{
    uint32 sig = si->m_sig;
    sigaction_t* action = &m_action[sig - 1];
    if (action->m_handler == SIG_DFL) {
        return handle_signal_default(sig);
    }

    /* get sig frame */
    sigframe_t* sig_frame = (sigframe_t *) ((frame->rsp - sizeof(sigframe_t)) & -8UL);

    /* save trap frame into sig frame */
    memcpy(&sig_frame->m_trap_frame, frame, sizeof(trap_frame_t));

    /* return addr */
    sig_frame->m_ret_addr = sig_frame->m_ret_code;

    /* return code: this is movl $,%eax ; int $0x80 */
    sig_frame->m_ret_code[0] = 0xb8;
    *(int *) (sig_frame->m_ret_code+1) = SIGRET;
    *(short *) (sig_frame->m_ret_code+5) = 0x80cd;

    /* set rip to sig handler */
    frame->rip = (uint64) action->m_handler;

    /* set signo to rdi */
    frame->rdi = si->m_sig;

    /* set esp as sig frame */
    frame->rsp = (uint64) sig_frame;

    return 0;
}

int32 signal_t::do_sigreturn(trap_frame_t* frame)
{
    sigframe_t* sig_frame = (sigframe_t *) (frame->rsp - 8);
    memcpy(frame, &sig_frame->m_trap_frame, sizeof(trap_frame_t));

    return 0;
}
