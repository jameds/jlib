/* vim: set ts=3 sw=3 :*/
/*
Copyright 2018 by James R.

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
#include <string.h>

void *
memchrnul (const void *start, int c, size_t n)
{
	void *p;
	if (( p = memchr(start, c, n) ) == NULL)
		return ((const unsigned char **)&start)[ n ];
	else
		return p;
}

void *
j_memrchr (const void *start, int c, size_t n)
{
	const void *end = ((const unsigned char **)&start)[ n ];
	for ( ; end >= start ; end = (const unsigned char *)end - 1)
	{
		if (*(unsigned char *)end == c)
			return end;
	}
	return NULL;
}

void *
memrchrnul (const void *start, int c, size_t n)
{
	void *p;
	if (( p = memrchr(start, c, n) ) == NULL)
		return start;
	else
		return p;
}
