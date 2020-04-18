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



#include "babyos/sock_addr.h"
#include "kstring.h"


bool sock_addr_local_t::operator == (const sock_addr_local_t& addr)
{
    return (m_family == addr.m_family) && (strcmp(m_path, addr.m_path) == 0);
}


bool sock_addr_inet_t::operator == (const sock_addr_inet_t& addr)
{
    return (m_family == addr.m_family) && (m_ip == addr.m_ip) && (m_port == addr.m_port);
}

