/*
 *	babyos/lib/libc/ctype.cc
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


#include <ctype.h>
#include <stdio.h>

__BEGIN_DECLS


const char _C_ctype_[1 + 256] = {
	0,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
   _S|(char)_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_N,	_N,	_N,	_N,	_N,	_N,	_N,	_N,
	_N,	_N,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P,
	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C,

	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* 80 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* 88 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* 90 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* 98 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* A0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* A8 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* B0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* B8 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* C0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* C8 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* D0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* D8 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* E0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* E8 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0, /* F0 */
	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0  /* F8 */
};

const char *_ctype_ = _C_ctype_;


int tolower(int c)
{
    if ((unsigned)c > 255) {
        return c;
    }

    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }

    return c;
}

int toupper(int c)
{
    if ((unsigned)c > 255) {
        return c;
    }

    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }

    return c;
}


int isalnum(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_U|_L|_N)));
}

int isalpha(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_U|_L)));
}

int isblank(int c)
{
	return (c == ' ' || c == '\t');
}

int iscntrl(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _C));
}

int isdigit(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _N));
}

int isgraph(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_P|_U|_L|_N)));
}

int islower(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _L));
}

int isprint(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_P|_U|_L|_N|_B)));
}

int ispunct(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _P));
}

int isspace(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _S));
}

int isupper(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _U));
}

int isxdigit(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_N|_X)));
}

int isascii(int c)
{
	return ((unsigned int)c <= 0177);
}

int toascii(int c)
{
	return (c & 0177);
}

__END_DECLS