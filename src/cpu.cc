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
#include "tss.h"


extern uint64 isr_vector[];

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



cpu_t::cpu_t()
{
}

cpu_t::~cpu_t()
{
}

void cpu_t::init()
{
}

void cpu_t::startup()
{
    init_gdt();
    init_idt();
    init_tss();
}

void cpu_t::init_gdt()
{
    set_global_descriptor(&m_gdt[SEG_NULL],  0x0000000000000000ULL);
    set_global_descriptor(&m_gdt[SEG_KCODE], 0x00af9a0000000000ULL);
    set_global_descriptor(&m_gdt[SEG_KDATA], 0x00cf920000000000ULL);
    set_global_descriptor(&m_gdt[SEG_UCODE], 0x00cffa0000000000ULL);
    set_global_descriptor(&m_gdt[SEG_UDATA], 0x00cff20000000000ULL);

    lgdt(m_gdt, sizeof(uint64) * (GDT_LEN));
}

void cpu_t::init_tss()
{
    memset(&m_tss, 0, sizeof(tss_t));

    /* set kstack */
    uint64 kstack = (uint64) PA2VA(os()->buddy()->alloc_pages(3));
    m_tss.x86_tss.sp0 = kstack;
    m_tss.x86_tss.sp1 = kstack;
    m_tss.x86_tss.sp2 = kstack;

    /* set interrupt stack table */
    uint64 ist = (uint64) PA2VA(os()->buddy()->alloc_pages(3));
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
            //current->m_vmm.do_page_fault(frame);
            return;
        }

        os()->console()->kprintf(RED, "Exception: %s\n", exception_msg[trapno]);
        //os()->console()->kprintf(RED, "current: %p, pid: %p\n", current, current->m_pid);
        os()->console()->kprintf(RED, "errno: %x, eip: %x, cs: %x, esp: %x\n",
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
        break;
    case IRQ_0 + IRQ_KEYBOARD:
        os()->keyboard()->do_irq();
        break;
    default:
        os()->console()->kprintf(RED, "Interrupt: %x not known.\n", trapno);
        break;
    }
}

void cpu_t::do_syscall(trap_frame_t* frame)
{
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

