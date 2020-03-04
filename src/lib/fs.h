/*
 *	babyos/kernel/fs.h
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
 *  2020-02-19		created
 */



#ifndef _FS_H_
#define _FS_H_


#include "types.h"
#include "file.h"
#include "inode.h"


#define MAX_PATH 14



typedef struct dir_entry_s {
    uint16 m_inum;
    char   m_name[MAX_PATH];
} dir_entry_t;


typedef struct stat_s {
    uint16 m_type;
    uint16 m_nlinks;
    uint32 m_dev;
    uint32 m_size;
} stat_t;


#endif
