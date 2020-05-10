/*
 *	babyos/kernel/file_descriptor_table.cc
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



#ifndef _FILE_TABLE_H_
#define _FILE_TABLE_H_

#include "babyos/types.h"
#include "fd.h"
#include "spinlock.h"

#define MAX_FILE_NUM 256


class file_descriptor_table_t {
public:
    void                init();
    file_descriptor_t*  alloc();
    int                 free(file_descriptor_t* file);
    file_descriptor_t*  dup_file(file_descriptor_t* file);

private:
    spinlock_t          m_lock;
    file_descriptor_t   m_file_descriptor_table[MAX_FILE_NUM];
};

#endif
