/*
 *	babyos/lib/libbdl/rect.cc
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
 *  2020-04-04		created
 */


#include <rect.h>


bool rect_t::empty() const
{
    return w <= 0 || h <= 0;
}

bool rect_t::intersect(rect_t& rhs, rect_t& result) const
{
    if (empty() || rhs.empty()) {
        result.w = 0;
        result.h = 0;
        return false;
    }

    /* horizontal intersection */
    int lhs_min = x;
    int lhs_max = lhs_min + w;
    int rhs_min = rhs.x;
    int rhs_max = rhs_min + rhs.w;

    if (rhs_min > lhs_min) {
        lhs_min = rhs_min;
    }
    result.x = lhs_min;
    if (rhs_max < lhs_max) {
        lhs_max = rhs_max;
    }
    result.w = lhs_max - lhs_min;

    /* vertical intersection */
    lhs_min = y;
    lhs_max = lhs_min + h;
    rhs_min = rhs.y;
    rhs_max = rhs_min + rhs.h;

    if (rhs_min > lhs_min) {
        lhs_min = rhs_min;
    }
    result.y = lhs_min;
    if (rhs_max < lhs_max) {
        lhs_max = rhs_max;
    }
    result.h = lhs_max - lhs_min;

    return !result.empty();
}

bool rect_t::has_intersection(const rect_t& rhs)
{
    if (this->empty() || rhs.empty()) {
        return false;
    }

    /* horizontal intersection */
    int lhs_min = x;
    int lhs_max = lhs_min + w;
    int rhs_min = rhs.x;
    int rhs_max = rhs_min + rhs.w;

    if (rhs_min > lhs_min) {
        lhs_min = rhs_min;
    }
    if (rhs_max < lhs_max) {
        lhs_max = rhs_max;
    }
    if (lhs_max <= lhs_min) {
        return false;
    }

    /* vertical intersection */
    lhs_min = y;
    lhs_max = lhs_min + h;
    rhs_min = rhs.y;
    rhs_max = rhs_min + rhs.h;

    if (rhs_min > lhs_min) {
        lhs_min = rhs_min;
    }
    if (rhs_max < lhs_max) {
        lhs_max = rhs_max;
    }
    if (lhs_max <= lhs_min) {
        return false;
    }

    return true;
}
