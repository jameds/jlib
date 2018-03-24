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

#include <j/ftr.h>

#include <stdlib.h>

#ifndef _WIN32
#if defined (__unix__) || defined (__unix) || defined (__MACH__)
#include <sys/stat.h>
#include <unistd.h>

int wread (int fd, void **buf, size_t bufn)
{
	struct stat st;

	if (fstat(fd, &st) == -1)
		return -1;

	if (*buf)
	{
		if (bufn < (size_t) st.st_size)
			if (realloc(*buf, st.st_size) == NULL)
				return -1;
	}
	else
	{
		*buf = malloc(st.st_size);
		if (*buf == NULL)
			return -1;
	}

	return read(fd, *buf, st.st_size);
}

static const size_t DEFAULT_BUFSIZE = 0x2000;
int swread (int fd, void **buf, size_t *bufn)
{
	void    *bufp;
	ssize_t  n;
	size_t   bn;

	if (*buf == NULL)
	{
		*bufn = DEFAULT_BUFSIZE;
		*buf  = malloc(*bufn);
		if (*buf == NULL)
			return -1;
	}

	bufp = *buf;
	bn   = *bufn;

	while ((n = read(fd, bufp, bn)) != 0)
	{
		if (n == -1)
			return -1;

		bufp += n;
		if (bufp == *buf + *bufn)
		{
			bn     = *bufn;
			*bufn *= 2;
			if (realloc(buf, *bufn) == NULL)
				return -1;
		}
		else
			bn -= n;
	}

	return bufp - *buf;
}
#endif
#endif

/* vim: set ts=3 sw=3 ft=c :*/
