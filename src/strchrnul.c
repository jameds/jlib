/*
	Copyright 2018 by James R.
	This file is a part of jlib.

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

#include <j/string.h>

char * strchrnul (const char *s, int c)
{
	while (*s != 0 && *s != c)
		;
	return s;
}

void * memrchr (const void *s, int c, size_t n)
{
	const unsigned char *p;
	for (p = (const unsigned char *)s + n; *p != c; --p)
	{
		if (p == s)
			return NULL;
	}
	return p;
}

void * memrchrnul (const void *s, int c, size_t n)
{
	const unsigned char *p;
	for (p = (const unsigned char *)s + n; p > s && *p != c; --p)
		;
	return p;
}
