/*
 *	babyos/include/dirent.h
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
 *  2020-03-12		created
 */


#ifndef _DIRENT_H_
#define _DIRENT_H_


#define MAX_PATH_LEN 14

typedef struct dir_entry_s {
    unsigned  short m_inum;
    char      m_name[MAX_PATH_LEN];
} dir_entry_t;


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2


#endif