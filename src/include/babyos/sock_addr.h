/*
 *	babyos/kernel/sock_addr.cc
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
 *  2020-02-26		created
 */




#ifndef _SOCK_ADDR_H_
#define _SOCK_ADDR_H_



#include "sock.h"

#define MAX_LOCAL_PATH      108


/* socket local */
class sock_addr_local_t : public sock_addr_t {
public:
    bool operator == (const sock_addr_local_t& addr);
    char m_path[MAX_LOCAL_PATH];
};


/* socket inet */
class sock_addr_inet_t : public sock_addr_t {
public:
    static const unsigned INADDR_ANY = 0;

    bool operator == (const sock_addr_inet_t& addr);
    unsigned m_ip;
    unsigned m_port;
};


#endif
