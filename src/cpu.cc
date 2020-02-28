/*
 *	babyos/kernel/cpu.cc
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


#include "cpu.h"
#include "x86.h"
#include "string.h"
#include "babyos.h"
#include "traps.h"
#include "syscall.h"


extern uint64 isr_vector[];
extern process_t* switch_to_asm(process_t* prev,
                                process_t* next,
                                uint64* prev_context_esp,
                                uint64* next_context_esp) __asm__ ("switch_to_asm");



static const char* exception_msg[] = {
    "int0  #DE divide error",
    "int1  #DB debug",
    "int2  --  NMI",
    "int3  #BP break point",
    "int4  #OF overflow",
    "int5  #BR bounds check",
    "int6  #UD invalid opcode",
    "int7  #NM device not available",
    "int8  #DF double fault",
    "int9  --  coprocessor seg overrun",
    "int10 #TS invalid TSS",
    "int11 #NP segment not present",
    "int12 #SS stack segment",
    "int13 #GP general protection",
    "int14 #PF page fault",
    "int15 -- （Intel reserved）",
    "int16 #MF x87 FPU coprocessor error",
    "int17 #AC align check",
};


extern "C" void do_common_isr(trap_frame_t* frame)
{
    os()->cpu()->do_common_isr(frame);
}

extern "C" void schedule()
{
    os()->cpu()->schedule();
}


extern "C" void schedule_tail(process_t* proc)
{
    os()->cpu()->schedule_tail(proc);
}

extern "C" process_t* __switch_to(process_t* prev, process_t* next)
{
    tss_t* tss = os()->cpu()->tss();
    tss->x86_tss.sp0 = (uint64)next->m_kstack;

    //os()->uart()->kprintf("switch %p(%d), %p -> %p(%d), %p\n",
    //                      prev, prev->m_pid, prev->m_context.rsp, next, next->m_pid, next->m_context.rsp);

    return prev;
}


#define switch_to(prev, next, last)                             \
    do {                                                        \
        ((last) = switch_to_asm(                                \
            (prev),                                             \
            (next),                                             \
            &(prev)->m_context.rsp,                             \
            &(next)->m_context.rsp)                             \
            );                                                  \
    } while (0)


/***************************************************************************/


cpu_t::cpu_t()
{
}

cpu_t::~cpu_t()
{
}

void cpu_t::init()
{
    m_kstack = (uint8 *) (KERNEL_STACK_BOTTOM);
    m_idle = (process_t *) (m_kstack - KERNEL_STACK_SIZE);
}

void cpu_t::startup()
{
    init_gdt();
    init_idt();
    init_tss();
    init_idle();
    m_local_apic.init();
}

void cpu_t::init_gdt()
{
    set_global_descriptor(&m_gdt[SEG_NULL],  0x0000000000000000ULL);
    set_global_descriptor(&m_gdt[SEG_KCODE], 0x0020980000000000ULL);
    set_global_descriptor(&m_gdt[SEG_KDATA], 0x0000920000000000ULL);
    set_global_descriptor(&m_gdt[SEG_UCODE], 0x0020f80000000000ULL);
    set_global_descriptor(&m_gdt[SEG_UDATA], 0x0000f20000000000ULL);

    lgdt(m_gdt, sizeof(uint64) * (GDT_LEN));
}

void cpu_t::init_tss()
{
    memset(&m_tss, 0, sizeof(tss_t));

    /* set kstack */
    m_tss.x86_tss.sp0 = (uint64) m_kstack;
    m_tss.x86_tss.sp1 = (uint64) m_kstack;
    m_tss.x86_tss.sp2 = (uint64) m_kstack;

    /* set interrupt stack table */
    uint64 ist = (uint64) P2V(os()->mm()->alloc_pages(3));
    for (int i = 0; i < 7; i++) {
        m_tss.x86_tss.ist[i] = ist;
    }

    /* set io bitmap */
    m_tss.x86_tss.io_bitmap_base = INVALID_IO_BITMAP_OFFSET;
    memset(&m_tss.io_bitmap, ~0, sizeof(uint64) * (IO_BITMAP_LONGS + 1));

    /* set descriptor */
    uint64 desc0, desc1;
    get_tss_descriptor((uint64)&m_tss, &desc0, &desc1);
    set_global_descriptor(&m_gdt[SEG_TSS0], desc0);
    set_global_descriptor(&m_gdt[SEG_TSS1], desc1);

    /* load tr */
    ltr(SEG_TSS0 << 3);
}

void cpu_t::init_idt()
{
    init_isrs();
    lidt(m_idt, sizeof(m_idt));
}

void cpu_t::init_isrs()
{
    /* exceptions */
    for (uint32 i = 0; i < IRQ_0; i++) {
        set_trap_gate(&m_idt[i], (uint64)isr_vector[i]);
    }

    /* interrupts */
    for (uint32 i = IRQ_0; i < 256; i++) {
        set_intr_gate(&m_idt[i], (uint64)isr_vector[i]);
    }

    /* syscall */
    set_system_gate(&m_idt[IRQ_SYSCALL], (uint64)isr_vector[IRQ_SYSCALL]);
}


void cpu_t::do_exception(trap_frame_t* frame)
{
    uint64 trapno = frame->trapno;

    if (trapno <= 0x10) {
        if (trapno == INT_PF) {
            current->m_vmm.do_page_fault(frame);
            return;
        }

        os()->console()->kprintf(RED, "Exception: %s, current: %p, errno: %x, rip: %p, cs: %p, rsp: %p\n",
                                 exception_msg[trapno], current, frame->err, frame->rip, frame->cs, frame->rsp);
        os()->uart()->kprintf("errno: %x, rip: %p, cs: %p, rsp: %p\n",
                                 frame->err, frame->rip, frame->cs, frame->rsp);
    }
    else {
        os()->console()->kprintf(RED, "Error Interrupt: %x, RESERVED!\n", trapno);
    }

    while (1) {
        halt();
    }
}

void cpu_t::do_interrupt(uint64 trapno)
{
    switch (trapno) {
    case IRQ_0 + IRQ_TIMER:
        os()->i8254()->do_irq();
        m_local_apic.eoi();
        break;
    case IRQ_0 + IRQ_KEYBOARD:
        os()->keyboard()->do_irq();
        m_local_apic.eoi();
        break;
    case IRQ_0 + IRQ_HARDDISK:
        os()->ide()->do_irq();
        m_local_apic.eoi();
        break;
    case VEC_LOCAL_TIMER:
        m_local_apic.do_timer_irq();
        m_local_apic.eoi();
        break;
    default:
        os()->console()->kprintf(RED, "Interrupt: %x not known.\n", trapno);
        break;
    }
}

void cpu_t::do_syscall(trap_frame_t* frame)
{
    syscall_t::do_syscall(frame);
}

void cpu_t::do_common_isr(trap_frame_t* frame)
{
    uint64 trapno = frame->trapno;
    if (trapno < IRQ_0) {
        do_exception(frame);
    }
    else if (trapno == IRQ_SYSCALL) {
        do_syscall(frame);
    }
    else {
        do_interrupt(trapno);
    }
}

void cpu_t::init_idle()
{
    os()->uart()->kprintf("init idle: %p\n", m_idle);

    m_idle->m_pid = 0;
    m_idle->m_kstack = m_kstack;
    memset(&m_idle->m_context, 0, sizeof(context_t));
    strcpy(m_idle->m_name, "idle");
    m_idle->m_state = process_t::RUNNING;
    m_idle->m_context.rsp = ((uint64) m_kstack);
    m_idle->m_timeslice = 2;
    m_idle->m_need_resched = 0;
    //m_idle->m_sig_queue.init(os()->get_obj_pool_of_size());
    //m_idle->m_sig_pending = 0;
    //m_idle->m_signal.init();

    m_idle->m_vmm.init();
    m_idle->m_vmm.set_pml4_table(os()->mm()->bootmem()->get_pml4());
    m_idle->m_children.init(os()->get_obj_pool_of_size());
    m_idle->m_wait_child.init();
    for (int i = 0; i < MAX_OPEN_FILE; i++) {
        m_idle->m_files[i] = NULL;
    }
}

process_t* cpu_t::get_idle_process()
{
    return m_idle;
}

void cpu_t::schedule()
{
    process_t* prev = current;
    process_t* next = m_idle;

    list_t<process_t *>* run_queue = os()->process_mgr()->get_run_queue();
    spinlock_t* rq_lock = os()->process_mgr()->get_rq_lock();
    uint64 flags;
    rq_lock->lock_irqsave(flags);

    list_t<process_t *>::iterator it = run_queue->begin();
    while (it != run_queue->end()) {
        process_t* p = *it;
        if (p->m_state == process_t::RUNNING && p->m_has_cpu == 0) {
            next = p;
            run_queue->erase(it);
            break;
        }
        it++;
    }

    prev->m_need_resched = 0;
    if (next == prev) {
        rq_lock->unlock_irqrestore(flags);
        return;
    }

    if (prev != next && prev->m_pid != 0 && prev->m_state == process_t::RUNNING) {
        if (run_queue->find(prev) == run_queue->end()) {
            run_queue->push_back(prev);
        }
    }

    next->m_has_cpu = 1;
    rq_lock->unlock_irqrestore(flags);

    /* switch mm */
    set_cr3(V2P(next->m_vmm.get_pml4_table()));

    /* switch registers and stack */
    switch_to(prev, next, prev);

    schedule_tail(prev);
}

void cpu_t::schedule_tail(process_t* proc)
{
    proc->lock();
    proc->m_has_cpu = 0;
    proc->unlock();
    mb();
}

tss_t* cpu_t::tss()
{
    return &m_tss;
}

local_apic_t* cpu_t::local_apic()
{
    return &m_local_apic;
}

void cpu_t::update()
{
    if (--current->m_timeslice == 0) {
        current->m_need_resched = 1;
        current->m_timeslice = 2;
    }
}

