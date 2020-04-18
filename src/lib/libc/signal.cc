/*
 *	babyos/lib/libc/signal.cc
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
 *  2020-03-16		created
 */



#include "stdint.h"
#include "signal.h"
#include "stdio.h"
#include "unistd.h"
#include "syscall_def.h"


sighandler_t signal(int signum, sighandler_t handler)
{
    return __signal(signum, handler);
}

int raise(int sig)
{
    return kill(getpid(), sig);
}

int killpg(int pgrp, int sig)
{
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}