/*
 *	babyos/lib/libbdl/bitmap.cc
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


#include <bitmap.h>
#include "stdio.h"
#include "string.h"
#include <cxx.h>

bitmap_t::bitmap_t()
{
    m_pitch = 0;
    m_pixels = nullptr;
}

bitmap_t::~bitmap_t()
{
    if (m_pixels != nullptr) {
        delete m_pixels;
    }
}

bool bitmap_t::load(const char* path)
{
    bool ret = false;
    uint8_t* p = nullptr;

    if (path == nullptr) {
        return false;
    }

    FILE* fp = fopen(path, "r");
    if (fp == nullptr) {
        printf("open %s failed\n", path);
        return false;
    }

    if (fread(&m_file_head, sizeof(bitmap_file_header_t), 1, fp) != 1) {
        printf("read file head failed\n");
        goto out;
    }

    if (strncmp(m_file_head.bf_magic, "BM", 2) != 0) {
        printf("check magic failed\n");
        goto out;
    }

    if (fread(&m_info_head, sizeof(bitmap_info_header_t), 1, fp) != 1) {
        printf("read info head failed\n");
        goto out;
    }

    if (m_info_head.bi_bit_count != 24) {
        printf("check bit count failed: %d\n", m_info_head.bi_bit_count);
        goto out;
    }

    m_pitch = ((m_info_head.bi_width * m_info_head.bi_bit_count + 31) >> 5) << 2;
    m_pixels = new uint8_t[m_pitch * m_info_head.bi_height];

    p = m_pixels;
    for (int y = 0; y < m_info_head.bi_height; y++) {
        if (fread(p, m_pitch, 1, fp) != 1) {
            printf("read pixels failed: %d\n", y);
            goto out;
        }
        p += m_pitch;
    }

    ret = true;

out:
    fclose(fp);

    return ret;
}


uint32_t bitmap_t::width()
{
    return m_info_head.bi_width;
}

uint32_t bitmap_t::height()
{
    return m_info_head.bi_height;
}

uint32_t bitmap_t::pitch()
{
    return m_pitch;
}

uint8_t* bitmap_t::pixels()
{
    return m_pixels;
}