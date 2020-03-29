/*
 *	babyos/kernel/vbe.h
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


#ifndef _VBE_H_
#define _VBE_H_

#include "types.h"
#include "color.h"


const int32 c_asc16_size = 16;
const int32 c_asc16_width = 8;
const int32 c_asc16_height = 16;

const int16 c_resolution_x = 1280;
const int16 c_resolution_y = 960;


class vbe_t {
	const uint16 c_bochs_port_index = 0x01ce;
	const uint16 c_bochs_port_data  = 0x01cf;

	enum {
		VBE_DISPI_INDEX_ID    		= 0,
		VBE_DISPI_INDEX_XRES  		= 1,
		VBE_DISPI_INDEX_YRES  		= 2,
		VBE_DISPI_INDEX_BPP   		= 3,
		VBE_DISPI_INDEX_ENABLE 		= 4,
		VBE_DISPI_INDEX_BANK        = 5,
		VBE_DISPI_INDEX_VIRT_WIDTH  = 6,
		VBE_DISPI_INDEX_VIRT_HEIGHT = 7,
		VBE_DISPI_INDEX_X_OFFSET 	= 8,
		VBE_DISPI_INDEX_Y_OFFSET 	= 9,
	};

public:
	vbe_t();
	~vbe_t();

	void   init();
	uint32 width();
	uint32 height();
	uint8* vram();

	void   set_pixel(uint32 x, uint32 y, color_ref_t color);
	void   draw_asc16(char ch, uint32 left, uint32 top, color_ref_t color);
	void   fill_rectangle(rect_t rect, color_ref_t color);
    void   scroll();

private:
    void   clip(rect_t* rect);
	void   load_asc16();
	void   bochs_graphic_init();
	void   bochs_set_resolution(uint32 resolution_x, uint32 resolution_y, uint32 bpp);
	void   bochs_vga_write(uint16 reg, uint16 val);
	uint16 bochs_vga_read(uint16 reg);

private:
	uint8*	m_base;        /* base address */
	uint8*	m_asc16_addr;
	uint16	m_width;
	uint16	m_height;
	uint8	m_bytes_pp;    /* bytes per pixel */
	uint32  m_video_mem_size;

	uint8*  m_ioaddr;
};

#endif
