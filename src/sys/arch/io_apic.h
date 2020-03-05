/*
 *	babyos/kernel/io_apic.h
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
 *  2020-02-27		created
 */



#ifndef _IO_APIC_H_
#define _IO_APIC_H_


#include "types.h"


#define IO_APIC_BASE     0xfec00000ull  /* default address map */


class io_apic_t {
public:
    void    init();
    void    enable_irq(uint32 irq, uint32 cpu_id);

private:
    uint32  read(uint32 reg);
    void    write(uint32 reg, uint32 data);

private:
};

#endif
