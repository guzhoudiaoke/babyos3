/*
 *	babyos/sys/arch/pci.h
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
 *  2020-03-29		created
 */



#ifndef _PCI_H_
#define _PCI_H_


#include "babyos/types.h"
#include "dlist.h"


class pci_device_bar_t {
public:
    void init(uint32 addr_reg_val, uint32 len_reg_val);
    void dump();

    enum type_e {
        TYPE_INVALID = -1,
        TYPE_MEM = 0,
        TYPE_IO,
    };

public:
    uint32 m_type;
    uint32 m_base_addr;
    uint32 m_length;
};

class pci_device_t {
public:
    void    init(uint8 bus, uint8 dev, uint8 function, uint16 vendor_id, uint16 device_id, 
                 uint32 class_code, uint8 revision, bool multi_function);
    void    dump();
    uint32  get_io_addr(int bar);
    uint32  get_io_length(int bar);
    uint32  get_interrupt_line();

public:
    uint8               m_bus;
    uint8               m_dev;
    uint8               m_function;

    uint16              m_vendor_id;
    uint16              m_device_id;
    uint32              m_class_code;
    bool                m_multi_function;
    uint8               m_interrupt_line;
    uint8               m_interrupt_pin;
    uint8               m_revision;
    pci_device_bar_t    m_bar[6];
    dlist_node_t        m_listnode;
};

class pci_t {
public:
    void init();
    pci_device_t* get_device(uint16 vendor_id, uint16 device_id);
    void enable_bus_mastering(pci_device_t* device);

private:
    void enum_buses();
    void check_device(uint8 bus, uint8 device);
    uint32 read(uint32 bus, uint32 device, uint32 function, uint32 addr);
    void write(uint32 bus, uint32 device, uint32 function, uint32 addr, uint32 val);

private:
    dlist_t             m_device_list;
};


#endif