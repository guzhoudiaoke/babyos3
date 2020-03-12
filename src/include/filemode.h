/*
 *	babyos/include/filemode.h
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


#ifndef _FILEMODE_H_
#define _FILEMODE_H_

enum file_mode_e {
    MODE_RDONLY = 0x1,
    MODE_WRONLY = 0x2,
    MODE_RDWR = 0x4,
    MODE_CREATE = 0x200,
};

enum inode_type_e {
    I_TYPE_DIR = 1,
    I_TYPE_FILE,
    I_TYPE_DEV,
};

#endif