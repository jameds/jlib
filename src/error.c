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

#include <j/error.h>

#include <stdarg.h>

int Error (const char *s, ...)
{
	va_list a;
	int     n;

	va_start(a, s);
	{
		/* Stream errors will likely persist. */
		if ((n = fprintf(stderr, "%s: ", __name)) < 0)
			goto ex;
		n += vfprintf(stderr, s, a);
		n++; putc('\n', stderr);
	}
ex:
	va_end(a);

	return n;
}

/* vim: set ts=3 sw=3 ft=c :*/
