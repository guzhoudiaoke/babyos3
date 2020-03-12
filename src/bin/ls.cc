/*
 *	babyos/user/ls.cc
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
 *  2020-02-20		created
 */



#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "dirent.h"


void get_name(const char* path, char* name)
{
    const char* p = path + strlen(path);
    while (*p == '/') {
        p--;
    }
    while (p >= path && *p != '/') {
        p--;
    }
    p++;

    strcpy(name, p);
}

void list_file(const char* name, unsigned size)
{
    printf("%20s %u\n", name, size);
}

void ls(const char* path)
{
    int fd = open(path, MODE_RDONLY);
    if (fd < 0) {
        printf("ls: cannot open %s\n", path);
        return;
    }

    stat_t st;
    if (fstat(fd, &st) < 0) {
        printf("ls: cannot stat file %s\n", fd);
        return;
    }

    static char name[32] = {0};
    dir_entry_t de;

    switch (st.m_type) {
    case I_TYPE_FILE:
        list_file(name, st.m_size);
        break;
    case I_TYPE_DIR:
        printf("%s: \n", path);
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.m_inum == 0) {
                continue;
            }

            char p[128] = {0};
            if (strcmp(de.m_name, ".") != 0 && strcmp(de.m_name, "..") != 0) {
                strcpy(p, path);
                if (*(p + strlen(p)) != '/') {
                    strcat(p, "/");
                }
            }
            strcat(p, de.m_name);

            if (stat(p, &st) < 0) {
                printf("ls: cannot stat %s\n", p);
                continue;
            }

            list_file(de.m_name, st.m_size);
        }
        break;
    }

    close(fd);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        ls(".");
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        ls(argv[i]);
    }

    exit(0);
    return 0;
}

