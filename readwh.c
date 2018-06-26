/* vim: set ts=3 sw=3 :*/
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

#include <j/ftr.h>

#include <stdio.h>
#include <stdlib.h>

static size_t grow_linear (size_t current, size_t needed, size_t block_size)
{
	(void)current;
	(void)block_size;
	return needed;
}

size_t
j_readwh (FILE *stream, void **buffer, size_t *buffer_size, size_t block_size)
{
	return j_readwhs(stream, buffer, buffer_size, block_size,
			&realloc, &grow_linear);
}

size_t
j_readwhs (FILE *stream, void **buffer, size_t *buffer_size, size_t block_size,
		void * (*realloc_f)(void *, size_t),
		size_t (*grow_f)(size_t current, size_t needed, size_t block_size))
{
	void *p;
#define try_realloc( z ) \
	{	if (( p = (*realloc_f)( *buffer, z ) ) == NULL) return 0;\
		*buffer      = p;\
		*buffer_size = z;\
	}

	size_t bytes, n;

	if (*buffer_size < block_size)
		try_realloc (block_size)

	for ( bytes = 0 ;; )
	{
		bytes +=
			( n = fread((unsigned char *)*buffer + bytes, 1, block_size, stream) );
		if (n < block_size)
		{
			     if (ferror (stream)) return 0;
			else if (feof   (stream)) return bytes;
		}

		if (*buffer_size < ( n = bytes + block_size ))
		{
			n = (*grow_f)( *buffer_size, n, block_size );
			try_realloc (n)
		}
	}
}
