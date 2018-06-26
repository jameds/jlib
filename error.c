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

#include <j/error.h>
#include <j/utility.h>  /* max() */
#include <stdarg.h>

int j_peprintf (const char *format, ...)
{
	va_list a;
	  int   n;

	/* Stream errors will likely persist. */
	if (( n = fprintf(stderr, "%s: ", j_program_name) ) < 0)
		return n;

	va_start (a, format);
	n += min (vfprintf(stderr, format, a), 0);
	va_end   (a);

	if (putc('\n', stderr))
		n++;

	return n;
}
