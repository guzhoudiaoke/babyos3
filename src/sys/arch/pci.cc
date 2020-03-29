/*
 *	babyos/sys/arch/pci.cc
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


#include "pci.h"
#include "babyos.h"
#include "x86.h"


#define PCI_CONFIG_ADDR                 0xCF8
#define PCI_CONFIG_DATA                 0xCFC

#define PCI_CONFIG_VENDOR               0x00
#define PCI_CONFIG_COMMAND              0x04
#define PCI_CONFIG_CLASS_REV            0x08
#define PCI_CONFIG_HDR_TYPE             0x0C
#define PCI_CONFIG_BASE_ADDR0           0x10
#define PCI_CONFIG_INTR                 0x3C

#define PCI_BASE_ADDR_MEM_MASK           (~0x0FUL)
#define PCI_BASE_ADDR_IO_MASK            (~0x03UL)



void pci_device_bar_t::init(uint32 addr_reg_val, uint32 len_reg_val)
{
    if (addr_reg_val == 0xffffffff) {
        addr_reg_val = 0;
    }

    if (addr_reg_val & 1) {
        m_type = TYPE_IO;
        m_base_addr = addr_reg_val  & PCI_BASE_ADDR_IO_MASK;
        m_length    = ~(len_reg_val & PCI_BASE_ADDR_IO_MASK) + 1;
    }
    else {
        m_type = TYPE_MEM;
        m_base_addr = addr_reg_val  & PCI_BASE_ADDR_MEM_MASK;
        m_length    = ~(len_reg_val & PCI_BASE_ADDR_MEM_MASK) + 1;
    }
}


void pci_device_bar_t::dump()
{
    os()->uart()->kprintf("type: %s, ", m_type == TYPE_IO ? "io base address" : "mem base address");
    os()->uart()->kprintf("base address: 0x%8x, ", m_base_addr);
    os()->uart()->kprintf("len: 0x%x\n", m_length);
}

void pci_device_t::init(uint8 bus, uint8 dev, uint8 function, uint16 vendor_id, uint16 device_id, 
                        uint32 class_code, uint8 revision, bool multi_function)
{
    m_bus = bus;
    m_dev = dev;
    m_function = function;

    m_vendor_id = vendor_id;
    m_device_id = device_id;
    m_multi_function = multi_function;
    m_class_code = class_code;
    m_revision = revision;

    for (int i = 0; i < 6; i++) {
        m_bar[i].m_type = pci_device_bar_t::TYPE_INVALID;
    }
    m_interrupt_line = -1;
}

void pci_device_t::dump()
{
    os()->uart()->kprintf("vendor id:      0x%x\n", m_vendor_id);
    os()->uart()->kprintf("device id:      0x%x\n", m_device_id);
    os()->uart()->kprintf("class code:     0x%x\n", m_class_code);
    os()->uart()->kprintf("revision:       0x%x\n", m_revision);
    os()->uart()->kprintf("multi function: %u\n", m_multi_function);
    os()->uart()->kprintf("interrupt line: %u\n", m_interrupt_line);
    os()->uart()->kprintf("interrupt pin:  %u\n", m_interrupt_pin);
    for (int i = 0; i < 6; i++) {
        if (m_bar[i].m_type != (uint32) pci_device_bar_t::TYPE_INVALID) {
            os()->uart()->kprintf("bar %u: \n", i);
            m_bar[i].dump();
        }
    }
}

uint32 pci_device_t::get_io_addr(int bar)
{
    if (m_bar[bar].m_type != (uint32) pci_device_bar_t::TYPE_INVALID) {
        return m_bar[bar].m_base_addr;
    }

    return 0;
}

uint32 pci_device_t::get_io_length(int bar)
{
    if (m_bar[bar].m_type != (uint32) pci_device_bar_t::TYPE_INVALID) {
        return m_bar[bar].m_length;
    }

    return 0;
}

uint32 pci_device_t::get_interrupt_line()
{
    return m_interrupt_line;
}

/*****************************************************************/

void pci_t::init()
{
    os()->uart()->kprintf("\n");
    os()->uart()->kprintf("********************** init pci **********************\n");

    m_device_list.init();
    enum_buses();

    os()->uart()->kprintf("********************** init pci **********************\n");
    os()->uart()->kprintf("\n");
}

void pci_t::enum_buses()
{
    for (uint16 bus = 0; bus < 256; bus++) {
        for (uint8 device = 0; device < 32; device++) {
            check_device(bus, device);
        }
    }
}

void pci_t::check_device(uint8 bus, uint8 device)
{
    uint8 function = 0;

    uint32 val = read(bus, device, function, PCI_CONFIG_VENDOR);
    uint16 vendor_id = val & 0xffff;
    uint16 device_id = val >> 16;
    if (vendor_id == 0xffff) {
        return;
    }

    val = read(bus, device, function, PCI_CONFIG_HDR_TYPE);
    uint8 header_type = ((val >> 16));

    //val = read(bus, device, function, PCI_CONFIG_COMMAND);
    //uint16 command = val & 0xffff;

    val = read(bus, device, function, PCI_CONFIG_CLASS_REV);
    uint32 classcode = val >> 8;
    uint8 revision = val & 0xff;

    pci_device_t *pci_device = (pci_device_t *) os()->mm()->kmalloc(sizeof(pci_device_t));
    pci_device->init(bus, device, function, vendor_id, device_id, classcode, revision, (header_type & 0x80));

    for (int bar = 0; bar < 6; bar++) {
        int reg = PCI_CONFIG_BASE_ADDR0 + (bar*4);
        val = read(bus, device, function, reg);
        write(bus, device, function, reg, 0xffffffff);
        uint32 len = read(bus, device, function, reg);
        write(bus, device, function, reg, val);

        if (len != 0 && len != 0xffffffff) {
            pci_device->m_bar[bar].init(val, len);
        }
    }

    val = read(bus, device, function, PCI_CONFIG_INTR);
    if ((val & 0xff) > 0 && (val & 0xff) < 32) {
        uint32 irq = val & 0xff;
        pci_device->m_interrupt_line = irq;
        pci_device->m_interrupt_pin = (irq >> 8);
    }

    os()->uart()->kprintf("pci device at bus: %u, device: %u\n", bus, device);
    pci_device->dump();
    m_device_list.add_tail(&pci_device->m_listnode);
}

uint32 pci_t::read(uint32 bus, uint32 device, uint32 function, uint32 addr)
{
    outl(PCI_CONFIG_ADDR, ((uint32) 0x80000000 | (bus << 16) | (device << 11) | (function << 8) | addr));
    return inl(PCI_CONFIG_DATA);
}

void pci_t::write(uint32 bus, uint32 device, uint32 function, uint32 addr, uint32 val)
{
    outl(PCI_CONFIG_ADDR, ((uint32) 0x80000000 | (bus << 16) | (device << 11) | (function << 8) | addr));
    outl(PCI_CONFIG_DATA, val);
}

pci_device_t* pci_t::get_device(uint16 vendor_id, uint16 device_id)
{
    dlist_node_t* node = m_device_list.head();
    while (node != nullptr) {
        pci_device_t* dev = list_entry(node, pci_device_t, m_listnode);
        if (dev->m_vendor_id == vendor_id && dev->m_device_id == device_id) {
            return dev;
        }
        node = node->next();
    }

    return NULL;
}

void pci_t::enable_bus_mastering(pci_device_t* device)
{
    uint32 val = read(device->m_bus, device->m_dev, device->m_function, PCI_CONFIG_COMMAND);
    val |= 4;
    write(device->m_bus, device->m_dev, device->m_function, PCI_CONFIG_COMMAND, val);

    val = read(device->m_bus, device->m_dev, device->m_function, PCI_CONFIG_COMMAND);
    os()->uart()->kprintf("after enable bus mastering, command: %x\n", val);
}
