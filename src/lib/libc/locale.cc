/*
 *	babyos/lib/libc/local.h
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


#include "locale.h"


static char default_decimal_point[] = ".";
static char default_thousands_sep[] = ",";
static char default_grouping[] = "\x03\x03";

static char default_empty_string[] = "";
static char default_empty_value = 127;

static struct lconv default_locale = {
    default_decimal_point,
    default_thousands_sep,
    default_grouping,
    default_empty_string,
    default_empty_string,
    default_empty_string,
    default_empty_string,
    default_empty_string,
    default_empty_string,
    default_empty_string,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value,
    default_empty_value
};



struct lconv* localeconv()
{
    return &default_locale;
}

extern char * setlocale(int category, const char *locale)
{
    return nullptr;
}
