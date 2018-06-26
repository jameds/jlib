/* vim: set ts=3 sw=3 :*/
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
#include <j/utility.h>
#include <string.h>

#define SWAP( a, b )\
{	a ^= b;\
	b ^= a;\
	a ^= b;\
}

void
j_strinrev (void *start, size_t n)
{
	void *end = ((unsigned char **)&start)[ n ];
	for ( ; end > start ;
			start = (unsigned char *)start + 1, end = (unsigned char *)end - 1)
	{
		SWAP (*(unsigned char *)start, *(unsigned char *)end);
	}
}

void *
j_strrev (void *dest, const void *start, size_t n)
{
	const void *end = ((const unsigned char **)&start)[ n ];
	for ( ; end >= start ;
			end = (const unsigned char *)end - 1, dest = (unsigned char *)dest + 1)
	{
		*(unsigned char *)dest = *(const unsigned char *)end;
	}
	return dest;
}

#define DEF( type_name, sizex, ... ) type_name (__VA_ARGS__) \
{	j_strinrev(s, sizex); return s; }
DEF (char *  strinrev,         strlen(s)-1,  char *s)
DEF (char * strninrev, zmin(n, strlen(s)-1), char *s, size_t n)
DEF (void *  meminrev,                    n, void *s, size_t n)
#undef DEF

char *
strrev (char *dest, const char *s)
{
	*(unsigned char *)
		j_strrev(dest, s, strlen(s)-1)
		= '\0';
	return dest;
}

char *
strnrev (char *dest, const char *s, size_t n)
{
	size_t len = strlen(s);
	if (n < len)
	{
		j_strrev(dest, s, n);
	}
	else
	{
		*(unsigned char *)
			j_strrev(dest, s, len)
			= '\0';
	}
	return dest;
}

void *
memrev (void *dest, const void *src, size_t n)
{
	j_strrev(dest, src, n);
	return dest;
}
