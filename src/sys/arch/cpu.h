/*
 *	babyos/kernel/cpu.h
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


#ifndef _CPU_H_
#define _CPU_H_


#include "babyos/types.h"
#include "descriptor.h"
#include "traps.h"
#include "process.h"
#include "local_apic.h"


class cpu_t {
public:
    cpu_t();
    ~cpu_t();

    void init();
    void startup();
    void update();

    void do_common_isr(trap_frame_t* frame);
    process_t* get_idle_process();
    void schedule();
    void schedule_tail(process_t* proc);
    tss_t* tss();
    local_apic_t* local_apic();


private:
    void init_gdt();
    void init_idt();
    void init_tss();
    void init_isrs();
    void init_idle();

    void do_exception(trap_frame_t* frame);
    void do_interrupt(uint64 trapno);
    void do_syscall(trap_frame_t* frame);


private:
    global_descriptor_t  m_gdt[GDT_LEN];
    gate_descriptor_s    m_idt[IDT_LEN];
    tss_t			     m_tss;

    uint8*               m_kstack;
    process_t*           m_idle;
    local_apic_t         m_local_apic;
};


#endif
