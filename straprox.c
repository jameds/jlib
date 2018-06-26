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

#include <j/string.h>
#include <string.h>
#include <j/utility.h>
#include <limits.h>

int
j_straprox (const void *lhs, const void *rhs, size_t n,
		int (*cmp_f)(const void *, const void *, size_t))
{
	/*
	Atleast on x86, this is faster than NSE (thanks to SSE).  A non-SSE implem-
	entation of strlen() and -ncmp() reveals how inefficient this truly is.  But
	I don't care to learn about SSE.
	 */
	return
		(( ((const unsigned char *)rhs)[n-1] == ((const unsigned char *)lhs)[n-1]
						&& (*cmp_f)((const void *)lhs, (const void *)rhs, n) <= 0 )
					? 0 : ~0 );
}

/*
Solve for v... (~=0) equal; (=0) differ or both zero
   Nonzero Simple Equality
	  v = a + b = a * 2
 */
/* OPTIMIZE ME !!! */
#if 0
int
j_strsaprox (const char *a, const char *b)
{
	/* NSE fails with mutual zero (0 + 0 = 0 * 2)--LUT adds 1. */
	static unsigned char MZE[ sizeof (unsigned char) << CHAR_BIT ] = { 1, };
	int c;
	for ( ;; a++, ++b)
	{
		c = *a + *b;
		if (( c | MZE[c] ) != ( *a << 1 ))
			return ( *a == '\0' ); /* a in b */
	}
}
#endif

#define DEF( name, sizex, cmp_f, ... ) int name (__VA_ARGS__) \
{	return j_straprox((const char *)lhs, (const char *)rhs, ( sizex ),\
		(int (*)(const void *, const void *, size_t))cmp_f); }
#define cc const char
#define cv const void
#define sz size_t
DEF (straprox,               strlen(lhs),     strncmp, cc *lhs, cc *rhs)
DEF (strnaprox,     zmin(n, strlen(lhs)),     strncmp, cc *lhs, cc *rhs, sz n)
DEF (memaprox,                         n,      memcmp, cv *lhs, cv *rhs, sz n)
DEF (strcaseaprox,           strlen(lhs), strncasecmp, cc *lhs, cc *rhs)
DEF (strncaseaprox, zmin(n, strlen(lhs)), strncasecmp, cc *lhs, cc *rhs, sz n)
#undef DEFINE

/*
An interesting thought experiment was to determine if two numbers are equal,
differ or one is zero.  In practice such an implementation of straprox() only
increases the instruction count.
*/
		/*
		Both formulas share the problem that they do not identify mutual zero.
		NZE fails with different lengths (e.g. comparing numbers--places differ).

		Solve for v... (=a) equal; (=b) differ; (=0) either zero
			Nonzero Switch (3 ins.)
			  nm = a XOR NOT b
			  v = nm AND a

		   Nonzero Extended Switch (4 ins.)
			  c = a OR b
			  v = c AND NOT ( A XOR B )
		 */
