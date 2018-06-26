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

#ifndef JLIB_STRING_H
#define JLIB_STRING_H

#include <j/version.h>
#include <string.h>

#include <stdlib.h>  /* size_t */

/*
strn- functions may be undesirable for use on memory areas not C strings.
Use mem- functions whenever possible for such input.
*/

/*
straprox() shall compare the two string lhs and rhs.   Zero is returned if
lhs matches even partially from the beginning of rhs. Otherwise, a nonzero
value is returned.
strnaprox() is similar, except that it compares at most only n bytes of
each string.
memaprox() is similar to strnaprox(), except that it compares memory areas.

strcaseaprox() and strncaseaprox() are similar, except that they compare
strings case insensitivitely.
*/
int straprox  (const char *lhs, const char *rhs);
int strnaprox (const char *lhs, const char *rhs, size_t n);
int memaprox  (const void *lhs, const void *rhs, size_t n);

int strcaseaprox  (const char *lhs, const char *rhs);
int strncaseaprox (const char *lhs, const char *rhs, size_t n);

/*
strrev() shall copy a reversed version of the string src into dest.   dest is
returned.
strnrev() is similar, except that it reverses and copies at most only n bytes
of src.
memrev() is similar to strnrev(), except that it reverses a memory area. memrev()

strinrev() and strninrev() reverse the given string in place.
meminrev() is similar to strninrev(), except that it reverses a memory area.

memrev() and meminrev() consider memory areas as bytes.
*/
char * strrev    (char *dest, const char *src);
char * strnrev   (char *dest, const char *src, size_t n);
void * memrev    (void *dest, const void *src, size_t n);

char * strinrev  (char *string);
char * strninrev (char *string, size_t n);
void * meminrev  (void *memory, size_t n);

/*
memchrnul() is similar to memchr(), except that upon failure, a pointer to
the nth byte from s is returned.

memrchr is similar to memchr(), except that it searches for a byte with the
value of c backward from the nth byte from s.
memrchrnul() is similar to memrchr(), except that upon failure, s is returned.
*/
void * memchrnul (const void *s, int c, size_t n);

#if !( (__GLIBC__ == 2 && __GLIBC_MINOR == 2) && defined (_GNU_SOURCE) )
extern void * j_memrchr (const void *, int, size_t);
#define memrchr j_memrchr
#endif
void * memrchrnul (const void *s, int c, size_t n);

/*
strcasecmp() is similar to strcmp(), except that it compares strings case in-
sensitivitely.
strncasecmp() is similar, except that it compares at most only n bytes of each
string.
*/
#define DECLJ \
int strcasecmp  (const char *lhs, const char *rhs);\
int strncasecmp (const char *lhs, const char *rhs, size_t n);
int memcasecmp  (const void *lhs, const void *rhs, size_t n);

#if defined (__unix__) || defined (__unix) || defined (__MACH__)
# include <unistd.h>
# if _POSIX_VERSION >= 200112L
#   include <strings.h>
/* From predef.sourceforce.net; 4.4BSD 199306 and BSD4_4. */
# elif !( defined (BSD) && BSD >= 199306 )
DECLJ
#define J__need_strcasecmp
# endif
#elif defined (_WIN32)
#   define strcasecmp  _stricmp
#   define strncasecmp _strnicmp
#else
DECLJ
#define J__need_strcasecmp
#endif

#endif
