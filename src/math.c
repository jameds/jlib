/*
	Copyright 2018 by James R.
	This file is part of jlib.

	jlib is free software; you can redistribute it and/or modify
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

#include <j/math.h>

#define i__AROUND \
	down = n * (x / n);                     \
	up   = down + n;                        \
	return (x - down < up - x) ? down : up; \

int around (int x, int n)
{
	int up, down;
	i__AROUND;
}

double faround (double x, double n)
{
	double up, down;
	i__AROUND;
}

float faroundf (float x, float n)
{
	float up, down;
	i__AROUND;
}

/* vim: set ts=3 sw=3 ft=c :*/
