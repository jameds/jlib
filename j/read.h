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

#ifndef JLIB_READ_H
#define JLIB_READ_H

#include <j/version.h>

#include <stdio.h>
#include <stdlib.h>  /* size_t */

/*
readwhs() shall read from the given stream into a buffer pointed to by bufferp.
blocksize bytes are read proceduraly until the end of the stream.  If the given
buffer is not large enough to store the contents of the stream,grow_f is called
to determine a new size for the buffer.  Then the buffer is reallocated to that
size by calling realloc_f.    buffer_sizep is updated with the true size of the
buffer.   The number of bytes read is returned.   Upon error, 0 is returned and
errno may be set.

readwh() is a pre-formated call to readwhs() that passes realloc() and a linear
growing function (merely returning the necessary bytes).
*/
size_t
j_readwh (FILE *stream, void **buffer, size_t *buffer_size, size_t block_size);
size_t
j_readwhs (FILE *stream, void **buffer, size_t *buffer_size, size_t block_size,
		void * (*realloc_f)(void *, size_t),
		size_t
		(*grow_f)(size_t current, size_t needed, size_t block_size));

#endif
