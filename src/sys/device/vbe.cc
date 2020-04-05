/*
 *	babyos/kernel/vbe.cc
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


#include "vbe.h"
#include "types.h"
#include "kernel.h"
#include "kstring.h"
#include "bootmem.h"
#include "babyos.h"
#include "x86.h"


vbe_t::vbe_t()
{
}

vbe_t::~vbe_t()
{
}

void vbe_t::init()
{
    video_info_t *info = os()->bootinfo()->video_info();
    m_width    = info->width;
    m_height   = info->height;
    m_bytes_pp = info->bits_per_pixel / 8;
    m_base     = (uint8 *)IO2V(info->vram_base_addr);
    m_ioaddr   = nullptr;
    m_buffer_id = 0;

    bochs_graphic_init();
    load_asc16();
}

void vbe_t::load_asc16()
{
    m_asc16_addr = (uint8 *) os()->mm()->kmalloc(4096);

    int fd = os()->fs()->do_open("/bin/ASC16", MODE_RDWR);
    if (fd < 0) {
        os()->panic("vbe init failed: not find");
    }

    if (os()->fs()->do_read(fd, m_asc16_addr, 4096) != 4096) {
        os()->panic("vbe init failed: size");
    }

    os()->fs()->do_close(fd);
}

uint32 vbe_t::width()
{
    return m_width;
}

uint32 vbe_t::height()
{
    return m_height;
}

uint32 vbe_t::depth()
{
    return m_bytes_pp;
}

uint32 vbe_t::vram_size()
{
    return m_video_mem_size;
}

uint8* vbe_t::vram()
{
    return m_base;
}

void vbe_t::set_pixel(uint32 x, uint32 y, color_ref_t color)
{
    if (x < m_width && y < m_height) {
        uint8* pvram = m_base + m_bytes_pp*y*m_width + m_bytes_pp*x;
        pvram[0] = RGB_GET_B(color);
        pvram[1] = RGB_GET_G(color);
        pvram[2] = RGB_GET_R(color);
    }
}

void vbe_t::draw_asc16(char ch, uint32 left, uint32 top, color_ref_t color)
{
    uint8* p_asc = m_asc16_addr + ch * c_asc16_size;

    for (int32 y = 0; y < c_asc16_height; y++) {
        uint8 test_bit = 1 << 7;
        for (int32 x = 0; x < c_asc16_width; x++) {
            if (*p_asc & test_bit) {
                set_pixel(left+x, top+y, color);
            }

            test_bit >>= 1;
        }
        p_asc++;
    }
}

void vbe_t::clip(rect_t* rect)
{
    uint32 right = rect->left + rect->width;
    uint32 bottom = rect->top + rect->height;
    if (right <= 0 || bottom <= 0) {
        rect->left = 0;
        rect->width = 0;
        rect->top = 0;
        rect->height = 0;
        return;
    }

    if (rect->left < 0) {
        rect->left = 0;
        rect->width = right - rect->left;
    }
    if (rect->top < 0) {
        rect->top = 0;
        rect->height = bottom - rect->top;
    }

    if (right > m_width) {
        rect->width = m_width - rect->left;
    }
    if (bottom > m_height) {
        rect->height = m_height - rect->top;
    }
}

void vbe_t::fill_rectangle(rect_t rect, color_ref_t color)
{
#if 0
    for (uint32 y = 0; y < rect.height; ++y)
    {
        for (uint32 x = 0; x < rect.width; ++x) {
            set_pixel(rect.left + x, rect.top + y, color);
        }
    }
#endif

    clip(&rect);
    if (rect.width == 0 || rect.height == 0) {
        return;
    }

    uint32 x = rect.left;
    uint8* pvram = m_base + m_bytes_pp*rect.top*m_width + m_bytes_pp*x;
    uint8* v = pvram;
    for (uint32 i = 0; i < rect.width; i++) {
        v[0] = RGB_GET_B(color);
        v[1] = RGB_GET_G(color);
        v[2] = RGB_GET_R(color);
        v += m_bytes_pp;
    }

    v = pvram + m_bytes_pp*m_width;
    for (uint32 y = rect.top+1; y < rect.top + rect.height; y++) {
        v = m_base + m_bytes_pp*y*m_width + m_bytes_pp*x;
        memcpy(v, pvram, m_bytes_pp*rect.width);
        v += m_bytes_pp*m_width;
    }
}

void vbe_t::scroll()
{
    memmove(m_base,
           m_base + m_width*m_bytes_pp*c_asc16_height,
           m_width*m_bytes_pp*(m_height-c_asc16_height));
}

/****************************************************************************************/

void vbe_t::bochs_vga_write(uint16 reg, uint16 val)
{
    if (m_ioaddr == nullptr) {
        outw(c_bochs_port_index, reg);
        outw(c_bochs_port_data, val);
    }
    else {
        *(uint16*) (m_ioaddr + 0x500 + (reg << 1)) = val;
    }
}

uint16 vbe_t::bochs_vga_read(uint16 reg)
{
    if (m_ioaddr == nullptr) {
        outw(c_bochs_port_index, reg);
        return inw(c_bochs_port_data);
    }
    else {
        return *(uint16*) (m_ioaddr + 0x500 + (reg << 1));
    }
}

void vbe_t::bochs_set_resolution(uint32 resolution_x, uint32 resolution_y, uint32 bpp)
{
    /* disable */
    bochs_vga_write(VBE_DISPI_INDEX_ENABLE, 0);

	/* set x resolution */
    bochs_vga_write(VBE_DISPI_INDEX_XRES, resolution_x);

	/* set y resolution */
	bochs_vga_write(VBE_DISPI_INDEX_YRES, resolution_y);

	/* set bpp to 32 */
	bochs_vga_write(VBE_DISPI_INDEX_BPP, bpp);

	/* set virtual height  */
    bochs_vga_write(VBE_DISPI_INDEX_VIRT_HEIGHT, resolution_y*2);

    /* enable */
    bochs_vga_write(VBE_DISPI_INDEX_ENABLE, 0x41);
}

void vbe_t::bochs_graphic_init()
{
    os()->uart()->kprintf("init bochs graphic\n");
    pci_device_t* device = os()->pci()->get_device(0x1234, 0x1111);
    if (device == NULL) {
        return;
    }

    /* enable bus mastering */
    os()->pci()->enable_bus_mastering(device);

    /* get io address */
    /* FIXME: why bar2 */
    uint64 ioaddr = device->get_io_addr(2);
    uint64 length = device->get_io_length(2);
    if (ioaddr != 0 && length != 0) {
        os()->uart()->kprintf("ioaddr: %p, length: %p\n", ioaddr, length);
        m_ioaddr = (uint8 *) IO2V(ioaddr);
        vmm_t::map_pages(os()->mm()->bootmem()->get_pml4(), 
                         m_ioaddr, ioaddr, length, PTE_W | PTE_U);
    }

    /* set resolution */
    bochs_set_resolution(c_resolution_x, c_resolution_y, 32);

    /* get resolution and so on */
    m_width = bochs_vga_read(VBE_DISPI_INDEX_XRES);
    m_height = bochs_vga_read(VBE_DISPI_INDEX_YRES);
    m_bytes_pp = bochs_vga_read(VBE_DISPI_INDEX_BPP) / 8;
    uint16 vh = bochs_vga_read(VBE_DISPI_INDEX_VIRT_HEIGHT);
    os()->uart()->kprintf("bochs graphic video resolution: %d * %d, %d, virtual height: %d\n", m_width, m_height, m_bytes_pp, vh);

    /* video ram address and length */
    uint64 addr = device->get_io_addr(0);
    m_video_mem_size = device->get_io_length(0);
    os()->uart()->kprintf("addr: %p, length: %p\n", addr, m_video_mem_size);

    if (addr != 0 && length != 0) {
        m_base = (uint8 *) IO2V(addr);
        vmm_t::map_pages(os()->mm()->bootmem()->get_pml4(), 
                         m_base, addr, m_video_mem_size, PTE_W | PTE_U);
    }
    os()->uart()->kprintf("bochs graphic vram base: %p, length: %p\n", m_base, m_video_mem_size);
}

void vbe_t::swap_buffer()
{
    m_buffer_id = !m_buffer_id;
    bochs_vga_write(VBE_DISPI_INDEX_Y_OFFSET, m_height*m_buffer_id);
}