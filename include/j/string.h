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

#ifndef JLIB_STRING_H
#define JLIB_STRING_H
#include <j/version.h>

#include <stddef.h>

/*
	Copy the first word, delimited by consecutive whitespace, from src into dest
	strnwcpy() and strnrwcpy() shall copy at most n characters.
	If n characters are reached, then the ending byte is not placed into dest.
	The reverse variants, strrwcpy() and strnrwcpy(), shall copy the last word.
 */
char * strwcpy   (char *dest, const char *src);
char * strnwcpy  (char *dest, const char *src, size_t n);
char * strrwcpy  (char *dest, const char *src);
char * strnrwcpy (char *dest, const char *src, size_t n);

/*
	strrev() shall copy a reversed version of the given string, src, into dest.
	strnrev() shall copy at most n characters.
	If n characters are reached, then the ending byte is not placed into dest.
	strrerev() shall reverse the given string in place.
 */
char * strrev    (char *dest, const char *src);
char * strnrev   (char *dest, const char *src, size_t n);
char * strrerev  (char *s);

/*
	Copies of GNU extensions.
	strchrnul() is no different than strchr(), except that upon failure, it will
	return a pointer to the end of s, rather than NULL.
	memrchr() shall search for the last occurrence of c in s, starting at the n-
	th character.
*/
char * strchrnul (const char *s, int c);
void * memrchr   (const void *s, int c, size_t n);

/*
	Identical to memrchr(), except that upon failure, s is returned, rather than
	NULL.
 */
void * memrchrnul (const void *s, int c, size_t n);

#endif
