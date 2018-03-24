/*
	Copyright 2018 by James R.

	This file is a part of jlib.

	jlib is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	jlib is distributed om the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with jlib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JLIB_FTR_H
#define JLIB_FTR_H
#include <j/version.h>

#include <stddef.h>

/*
	Read the whole of a file into a buffer.
	If the buffer is too small to fit the file byte count, it shall be resized.
	If the value pointed to by buf is NULL, a new buffer
	shall be allocated in its place.

	swread() is a stream safe alternative to wread().
	It will likely allocate more than is needed, and should be reallocated
	for long term use.

	On failure, -1 is returned and errno may be set.
	Otherwise, returned is the number of bytes read from the file.
	swread() additionally sets the value pointed to by bufn to the size
	of the allocated buffer.
 */
int wread  (int fd, void **buf, size_t  bufn);
int swread (int fd, void **buf, size_t *bufn);

#endif
