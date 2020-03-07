/*
 *	babyos/kernel/babyos.cc
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

#include "babyos.h"
#include "x86.h"
#include "delay.h"
#include "syscall.h"
#include "pipe.h"
#include "syscall.h"
#include "sys_socket.h"


static babyos_t babyos;


void test_ide();
void test_syscall();
void test_fs();


babyos_t* babyos_t::get_instance()
{
    return &babyos;
}

babyos_t::babyos_t()
{
}

babyos_t::~babyos_t()
{
}

uart_t* babyos_t::uart()
{
    return &m_uart;
}

vbe_t* babyos_t::vbe()
{
    return &m_vbe;
}

console_t* babyos_t::console()
{
    return &m_console;
}

bootinfo_t* babyos_t::bootinfo()
{
    return &m_bootinfo;
}

mm_t* babyos_t::mm()
{
    return &m_mm;
}

i8259a_t* babyos_t::i8259a()
{
    return &m_i8259a;
}

i8254_t* babyos_t::i8254()
{
    return &m_i8254;
}

cpu_t* babyos_t::cpu()
{
    return &m_cpu;
}

rtc_t* babyos_t::rtc()
{
    return &m_rtc;
}

keyboard_t* babyos_t::keyboard()
{
    return &m_keyboard;
}

ide_t* babyos_t::ide()
{
    return &m_ide;
}

process_mgr_t* babyos_t::process_mgr()
{
    return &m_process_mgr;
}

timer_mgr_t* babyos_t::timer_mgr()
{
    return &m_timer_mgr;
}

block_dev_t* babyos_t::block_dev()
{
    return &m_block_dev;
}

dev_op_t* babyos_t::get_dev(uint32 type)
{
    if (type >= MAX_DEV) {
        return NULL;
    }
    return &m_devices[type];
}

file_system_t* babyos_t::fs()
{
    return &m_fs;
}

io_apic_t* babyos_t::io_apic()
{
    return &m_io_apic;
}


void babyos_t::init()
{
    m_panic = false;
    delay_t::init(1000000000ULL);

    /* serial port */
    m_uart.early_init();
    uart()->puts("Hello babyos..\n");

    /* bootinfo */
    m_bootinfo.init();
    uart()->puts("boot info init done\n");

    /* mm */
    m_mm.init();

    /* 8259a */
    m_i8259a.init();
    uart()->puts("8259a init done\n");

    /* 8254 */
    m_i8254.init();
    uart()->puts("8254 init done\n");

    /* cpu */
    m_cpu.init();
    m_cpu.startup();
    uart()->puts("cpu startup done\n");

    /* io apic */
    m_io_apic.init();
    uart()->puts("io apic init done\n");

    /* rtc */
    m_rtc.init();
    uart()->puts("rtc init done\n");

    /*keyboard */
    m_keyboard.init();
    uart()->puts("keyboard init done\n");

    /* ide */
    m_ide.init(1);
    uart()->puts("ide init done\n");

    /* block dev */
    m_block_dev.init(1);
    uart()->puts("block dev init done\n");

    /* syscall */
    syscall_t::init();
    uart()->puts("syscall init done\n");

    /* socket */
    sys_socket_t::init();
    uart()->puts("socket init done\n");

    /* fs */
    m_fs.init();
    uart()->puts("fs init done\n");

    /* timer manager */
    m_timer_mgr.init();

    /* process manager */
    m_process_mgr.init();

    /* start interrupt */
    sti();
    uart()->puts("sti done\n");

    /* set cwd as root */
    current->set_cwd(os()->fs()->get_root());

    /* VBE */
    m_vbe.init();
    uart()->puts("VBE init done\n");

    /* console */
    m_console.init();
    uart()->puts("console init done\n");
    console()->kprintf(YELLOW, "Welcome to babyos!\n");

    /* start the first user process: init */
    start_init_proc();
    uart()->puts("start init proc\n");

    //test_syscall();
    //test_ide();
    //test_fs();
}

void babyos_t::start_init_proc()
{
    int32 ret = 0;
    __asm__ volatile("int $0x80" : "=a" (ret) : "a" (syscall_t::FORK));

    if (ret == 0) {
        __asm__ volatile("int $0x80"
                         : "=a" (ret)
                         : "a" (syscall_t::EXEC), "D" ("/bin/init"), "S" (0));
        os()->panic("exec return!\n");
    }
}

void babyos_t::run()
{
    while (true) {
        if (m_panic) {
            cli();
            while (1) {
                halt();
            }
        }

        while (process_mgr()->get_run_queue()->empty()) {
            nop();
        }
        sti();
        cpu()->schedule();
    }
}

void show_time_now()
{
    uint32 year, month, day, h, m, s;
    year  = os()->rtc()->year();
    month = os()->rtc()->month();
    day   = os()->rtc()->day();

    h     = os()->rtc()->hour();
    m     = os()->rtc()->minute();
    s     = os()->rtc()->second();
    os()->uart()->kprintf("%d-%d-%d %2d:%2d:%2d\n", 2000+year, month, day, h, m, s);
}

void babyos_t::update(uint64 tick)
{
    cpu()->update();
    rtc()->update();
    timer_mgr()->update();

    if (tick % 100 == 0) {
        show_time_now();
    }
}

void babyos_t::panic(const char* s)
{
    cli();
    m_uart.kprintf("[BABYOS PANICED]: %s\n", s);
    while (1) {
        halt();
    }
}


/********************************************************************/

void test_syscall()
{
    char str[100] = "Hello babyos, this is printed by syscall\n";
    os()->uart()->puts("test syscall\n");
    __asm__ volatile("int $0x80" : : "D" (WHITE), "S" (str), "a" (syscall_t::PRINT));
}

void test_ide()
{
    os()->uart()->puts("test ide\n");

    io_buffer_t buffer;
    memset(buffer.m_buffer, 0, 512);
    request_t req;
    req.init(0, 0, request_t::CMD_READ, &buffer);
    os()->ide()->add_request(&req);

    while (!buffer.m_done) {
        halt();
    }

    os()->uart()->puts("test ide, read done\n");

    for (int i = 0; i < SECT_SIZE/2; i++) {
        if (i % 8 == 0) {
            os()->console()->kprintf(PINK, "\n");
        }
        os()->console()->kprintf(PINK, "%4x ", ((uint16 *)buffer.m_buffer)[i]);
    }
    os()->console()->kprintf(PINK, "\n");
}

void test_fs()
{
    current->set_cwd(os()->fs()->get_root());

    os()->uart()->kprintf("before open\n");
    int fd = os()->fs()->do_open("/bin/test", file_t::MODE_RDWR);
    os()->uart()->kprintf("after open: %d\n", fd);
    if (fd < 0) {
        os()->panic("BUG on open file test!\n");
    }

    // 2. read elf from hard disk
    char buf[2048];
    memset(buf, 0, 2048);
    os()->uart()->kprintf("before read: %d\n", fd);
    if (os()->fs()->do_read(fd, buf, 2048) < 0) {
        os()->panic("BUG on read file test!\n");
    }

    os()->uart()->kprintf("after read: %d\n", fd);
    os()->console()->kprintf(YELLOW, "\n\nThe content of /bin/test\n");
    os()->console()->kprintf(CYAN, "%s", buf);
}
