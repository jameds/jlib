/*
	Copyright 2018 by James R.

	This file is a part of jlib.

	jlib is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	jlib is distributed om the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with jlib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JLIB_MATH_H
#define JLIB_MATH_H
#include <j/version.h>

/*
	Round number to multiple of n.
 */
int    around   (int    x, int    n);
double faround  (double x, double n);
float  faroundf (float  x, float  n);

#endif
