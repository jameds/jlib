/* vim: set ts=3 sw=3 :*/
/*
Copyright 2018 by James R.

The file is a part of jlib.

jlib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

jlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with jlib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <j/utility.h>

#include <math.h>

#define MOD(x, y) ( (x) % (y) )

#define DEFINE(type, name, MIN_or_MAX) type name (type a, type b) \
{	return MIN_or_MAX (a, b);\
}
DEFINE            (long,  lmin,     MIN)
DEFINE            (long,  lmax,     MAX)
DEFINE (unsigned   long, ulmin,     MIN)
DEFINE (unsigned   long, ulmax,     MAX)
DEFINE          (size_t,  zmin,     MIN)
DEFINE          (size_t,  zmax,     MAX)
#undef  DEFINE

#define DEFINE(type, name, remm, expr) type name (type x, type y) \
{	type r = remm (x, y);\
	return expr - r;\
}
#define ROUND_expr ( (r >= y / 2) ? x + y : x )
#define FLOOR_expr ( x )
#define  CEIL_expr ( x + y )
DEFINE            (long,  laround,  MOD,        ROUND_expr)
DEFINE            (long,  lafloor,  MOD,        FLOOR_expr)
DEFINE            (long,  laceil,   MOD,         CEIL_expr)
DEFINE (unsigned   long, ularound,  MOD,        ROUND_expr)
DEFINE (unsigned   long, ulafloor,  MOD,        FLOOR_expr)
DEFINE (unsigned   long, ulaceil,   MOD,         CEIL_expr)

DEFINE          (double,  faround,  remainder,  ROUND_expr)
DEFINE          (double,  fafloor,  remainder,  FLOOR_expr)
DEFINE          (double,  faceil,   remainder,   CEIL_expr)
DEFINE           (float,  faroundf, remainderf, ROUND_expr)
DEFINE           (float,  fafloorf, remainderf, FLOOR_expr)
DEFINE           (float,  faceilf,  remainderf,  CEIL_expr)
#undef  DEFINE
