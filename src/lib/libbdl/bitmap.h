/*
 *	babyos/lib/libbdl/bitmap.h
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
 *  2020-04-09		created
 */


#ifndef _BITMAP_H_
#define _BITMAP_H_


#include "types.h"


#pragma pack(1)
typedef struct bitmap_file_header_t {
	char     bf_magic[2];
	uint32_t bf_size;
	uint16_t bf_reserved1;
	uint16_t bf_reserved2;
	uint32_t bf_offset_bits;
} bitmap_file_header_t;

typedef struct bitmap_info_header_t {
	uint32_t bi_size;
	int32_t  bi_width;
	int32_t  bi_height;
	uint16_t bi_planes;
	uint16_t bi_bit_count;
	uint32_t bi_compression;
	uint32_t bi_size_image;
	int32_t  bi_xpels_per_meter;
	int32_t  bi_ypels_per_meter;
	uint32_t bi_clr_used;
	uint32_t bi_clr_important;
} bitmap_info_header_t;
#pragma pack()


class bitmap_t {
public:
    bitmap_t();
    ~bitmap_t();

    bool load(const char* path);
    uint32_t width();
    uint32_t height();
    uint32_t pitch();
    uint8_t* pixels();

private:
    bitmap_file_header_t m_file_head;
    bitmap_info_header_t m_info_head;
    uint32_t             m_pitch;
    uint8_t*             m_pixels;
};


#endif