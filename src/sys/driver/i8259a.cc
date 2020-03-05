/*
 *	babyos/kernel/i8259a.cc
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


#include "i8259a.h"
#include "x86.h"


i8259a_t::i8259a_t()
{
}

i8259a_t::~i8259a_t()
{
}

void i8259a_t::init()
{
    /* ICW1：0x11, 表示边沿触发、多片级联、需要发送ICW4 */
    outb(0x20, 0x11);
    outb(0xa0, 0x11);

    /* ICW2：
     * 重新映射 IRQ0~IRQ7 到 0x20~0x27,重新映射 IRQ8~IRQ15 到 0x28~0x2F */
    outb(0x21, 0x20);
    outb(0xa1, 0x28);

    /* ICW3:
     * 	     A0	D7  D6  D5  D4  D3  D2  D1  D0
     * master 1	S7  S6	S5  S4  S3  S2  S1  S0
     * slave  1	0   0   0   0   0   ID2 ID1 ID0
     * master: 0x04，表示主芯片的IR2引脚连接一个从芯片
     * slave: 0x02，表示从片连接到主片的IR2 引脚 */
    outb(0x21, 0x04);
    outb(0xa1, 0x02);

    /* 从片连接到主片的 IRQ2 */
    /* ICW4：0x01，表示8259A芯片被设置成普通嵌套、非中断方式、
     * 用于8086 及其兼容模式 */
    outb(0x21, 0x01);
    outb(0xa1, 0x01);

    /* disable all interrupts */
    outb(0x21, 0xff);
    outb(0xa1, 0xff);
}

void i8259a_t::enable_irq(uint32 irq)
{
    uint8 mask;

    if (irq < 8)
    {
        mask = inb(0x21) & (0xff ^ (1 << irq));
        outb(0x21, mask);
    }
    else if (irq < 16)
    {
        mask = inb(0x21) & 0xfb;
        outb(0x21, mask);

        mask = inb(0xa1) & (0xff ^ (1 << (irq-8)));
        outb(0xa1, mask);
    }
}

