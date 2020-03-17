/*
 *	babyos/lib/libc/math.h
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


#ifndef _MATH_H_
#define _MATH_H_


#include "sys/cdefs.h"


#define HUGE_VAL (__builtin_huge_val())
#define M_E 2.718281828459045
#define M_PI 3.141592653589793
#define M_PI_2 (M_PI / 2)
#define M_TAU (M_PI * 2)
#define M_LN2 0.69314718055995
#define M_LN10 2.30258509299405


__BEGIN_DECLS

double acos(double);
float acosf(float);
double asin(double);
float asinf(float);
double atan(double);
float atanf(float);
double atan2(double, double);
float atan2f(float, float);
double cos(double);
float cosf(float);
double cosh(double);
float coshf(float);
double sin(double);
float sinf(float);
double sinh(double);
float sinhf(float);
double tan(double);
float tanf(float);
double tanh(double);
float tanhf(float);
double ceil(double);
float ceilf(float);
double floor(double);
float floorf(float);
double round(double);
float roundf(float);
double fabs(double);
float fabsf(float);
double fmod(double, double);
float fmodf(float, float);
double exp(double);
float expf(float);
double frexp(double, int* exp);
float frexpf(float, int* exp);
double log(double);
float logf(float);
double log10(double);
float log10f(float);
double sqrt(double);
float sqrtf(float);
double modf(double, double*);
float modff(float, float*);
double ldexp(double, int exp);
float ldexpf(float, int exp);
double pow(double x, double y);
float powf(float x, float y);
double log2(double);
float log2f(float);
long double log2l(long double);
double frexp(double, int*);
float frexpf(float, int*);
long double frexpl(long double, int*);

double trunc(double x);
double copysign(double, double);
float copysignf(float, float);
float scalbnf(float, int);
double scalbn(double, int);

double expm1(double x);


__END_DECLS

#endif
