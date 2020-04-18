/*
 *	babyos/kernel/babyos.h
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
 *  2020-02-13		created
 */


#ifndef _BABYOS_H_
#define _BABYOS_H_


#include "types.h"
#include "uart.h"
#include "vbe.h"
#include "console.h"
#include "bootmem.h"
#include "mm.h"
#include "bootinfo.h"
#include "i8259a.h"
#include "i8254.h"
#include "cpu.h"
#include "rtc.h"
#include "keyboard.h"
#include "ide.h"
#include "pool.h"
#include "process_mgr.h"
#include "timer_mgr.h"
#include "block_dev.h"
#include "fs.h"
#include "io_apic.h"
#include "pci.h"
#include "framebuffer.h"
#include "fpu.h"
#include "mouse.h"


enum device_type_e {
    DEV_CONSOLE = 0,
    DEV_FRAMEBUFFER = 1,
    MAX_DEV,
};


class babyos_t {
public:
    static babyos_t* get_instance();

    babyos_t();
    ~babyos_t();

    void init();
    void run();
    void update(uint64 tick);
    void panic(const char* s);

    uart_t*         uart();
    mm_t*           mm();
    vbe_t*          vbe();
    console_t*      console();
    bootinfo_t*     bootinfo();
    i8259a_t*       i8259a();
    i8254_t*        i8254();
    cpu_t*          cpu();
    rtc_t*          rtc();
    keyboard_t*     keyboard();
    mouse_t*        mouse();
    ide_t*          ide();
    process_mgr_t*  process_mgr();
    timer_mgr_t*    timer_mgr();
    block_dev_t*    block_dev();
    dev_op_t*       get_dev(uint32 type);
    file_system_t*  fs();
    io_apic_t*      io_apic();
    pci_t*          pci();
    framebuffer_t*  framebuffer();

private:
    void            start_init_proc();

private:
    bool           m_panic;
    uart_t         m_uart;
    bootinfo_t     m_bootinfo;
    mm_t           m_mm;
    vbe_t          m_vbe;
    console_t      m_console;
    i8259a_t       m_i8259a;
    i8254_t        m_i8254;
    cpu_t          m_cpu;
    rtc_t          m_rtc;
    keyboard_t     m_keyboard;
    mouse_t        m_mouse;
    ide_t          m_ide;
    block_dev_t    m_block_dev;
    file_system_t  m_fs;
    io_apic_t      m_io_apic;
    pci_t          m_pci;
    framebuffer_t  m_framebuffer;
    fpu_t          m_fpu;

    process_mgr_t  m_process_mgr;
    timer_mgr_t    m_timer_mgr;
    dev_op_t       m_devices[MAX_DEV];
};

#define os() babyos_t::get_instance()


#endif
