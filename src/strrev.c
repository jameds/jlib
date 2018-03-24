/*
	Copyright 2018 by James R.

	This file is a part of jlib.

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

#include <j/string.h>

#define SWAP(a, b) {  a ^= b; b ^= a; a ^= b; }

char * strrev (char *dest, const char *src)
{
	register const char *p;
	register char       *pd;

	for (p  = src;  *p; ++p) ;
	for (pd = dest;  p-- > src; )
		*pd++ = *p;
	*pd = 0;

	return dest;
}

char * strnrev (char *dest, const char *src, size_t n)
{
	register const char *p;
	register char       *pd;

	for (p  = src;  n && *p; ++p, --n) ;
	for (pd = dest; p-- > src; )
		*pd++ = *p;
	if (n)
		*pd = 0;

	return dest;
}

char * strrerev (char *s)
{
	register char *pd, *p;

	for (p  = s;  *p; ++p) ;
	for (pd = s; --p > pd; )
	{
		SWAP(*pd, *p);
		pd++;
	}

	return s;
}

/* vim: set ts=3 sw=3 ft=c :*/
