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
#include "file_table.h"
#include "inode.h"
#include "sock_addr.h"
#include "stat.h"
#include "dirent.h"


#define ROOT_DEV 1
#define ROOT_INUM 1

#define BSIZE   2048

#define MAX_INODE_CACHE 64



typedef struct super_block_s {
    uint32 m_size;      /* total num of blocks */
    uint32 m_nblocks;   /* num of data blocks */
    uint32 m_ninodes;   /* num of inodes */
} super_block_t;



class file_system_t {
public:
    void     init();
    inode_t* get_root();
    uint32   inode_block(uint32 id);
    uint32   inode_offset(uint32 id);

    int      do_open(const char* path, int mode);
    int      do_close(int fd);
    int64    do_read(int fd, void* buffer, uint32 count);
    int64    do_write(int fd, void* buffer, uint32 count);
    int      do_mkdir(const char* path);
    int      do_link(const char* path_old, const char* path_new);
    int      do_unlink(const char* path);
    int      do_mknod(const char* path, int major, int minor);
    int      do_dup(int fd);
    int      do_seek(int fd, uint64 pos, int whence);
    int      do_stat(int fd, stat_t* st);
    int      do_chdir(const char* path);
    int      do_pipe(int fd[2]);
    int      do_ioctl(int fd, int cmd, uint64 arg);
    int      do_send_to(int fd, void* buffer, uint64 count, sock_addr_t* addr);
    int      do_recv_from(int fd, void* buffer, uint64 count, sock_addr_t* addr);

    inode_t* dup_inode(inode_t* inode);
    void     put_inode(inode_t* inode);

    file_t*  dup_file(file_t* file);
    file_t*  alloc_file();
    int      close_file(file_t* file);

private:
    uint32   bitmap_block();

    void     read_super_block(super_block_t* sb);
    void     zero_block(uint32 dev, uint32 b);
    uint32   alloc_block(uint32 dev);
    void     free_block(uint32 dev, uint32 b);
    uint32   block_map(inode_t* inode, uint32 block);

    inode_t* alloc_inode(uint16 dev, uint16 type);
    inode_t* get_inode(uint32 dev, uint32 inum);
    int      read_inode(inode_t* inode, void* dst, uint64 offset, uint64 size);
    int      write_inode(inode_t* inode, void* src, uint64 offset, uint64 size);

    int      dir_link(inode_t* inode, char* name, uint32 inum);
    inode_t* dir_lookup(inode_t* inode, char* name, unsigned& offset);
    bool     dir_empty(inode_t* inode);

    inode_t* namei(const char* path);
    inode_t* nameiparent(const char* path, char *name);
    inode_t* namei(const char* path, int parent, char* name);
    inode_t* create(const char* path, uint16 type, uint16 major, uint16 minor);

    int      alloc_pipe(file_t*& file_read, file_t*& file_write);

private:
    uint32          m_dev;
    uint32          m_super_block_lba;
    uint32          m_inode_lba;

    super_block_t   m_super_block;
    inode_t*        m_root;

    spinlock_t      m_inodes_lock;
    inode_t         m_inodes[MAX_INODE_CACHE];

    file_table_t    m_file_table;
};


#endif
