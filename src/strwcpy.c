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

#include <ctype.h>

char * strwcpy (char *dest, const char *src)
{
	register char *pd;

	for (;          *src &&   isblank(*src); ++src) ;
	for (pd = dest; *src && ! isblank(*src); ++src)
		*pd++ = *src;
	*pd = 0;

	return dest;
}

char * strnwcpy (char *dest, const char *src, size_t n)
{
	register char *pd;

	for (;               *src &&   isblank(*src); ++src) ;
	for (pd = dest; n && *src && ! isblank(*src); ++src, --n)
		*pd++ = *src;
	if (n)
		*pd = 0;

	return dest;
}

#define i__RWC \
		if (! isblank(*src)) {     \
			if (pe+1 != src)        \
				p = src;             \
			pe = src;         }     \
	for (pd = dest; p <= pe; ++p) \
		*pd++ = *p;

char * strrwcpy (char *dest, const char *src)
{
	register char       *pd;
	register const char *p, *pe;

	for (p = pe = src; *src; ++src)
	i__RWC;
	*pd = 0;

	return dest;
}

char * strnrwcpy (char *dest, const char *src, size_t n)
{
	register char       *pd;
	register const char *p, *pe;

	for (p = pe = src; n && *src; ++src, --n)
	i__RWC;
	if (n)
		*pd = 0;

	return dest;
}

/* vim: set ts=3 sw=3 ft=c :*/
