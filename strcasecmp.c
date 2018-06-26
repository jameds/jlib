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
#include <ctype.h>
#include <string.h>
#include <j/utility.h>

#define cucp( id ) ( (const unsigned char *)( id ) )
#define BODY( expr ) \
	for ( ; expr ; lhs = cucp (lhs) + 1, rhs = cucp (rhs) + 1) \
{	if (toupper(*cucp (lhs)) != toupper(*cucp (rhs)))\
		return ( *cucp (lhs) - *cucp (rhs) );\
}\
return 0;

# ifdef J__need_strcasecmp
int
strcasecmp (const char *lhs, const char *rhs)
{
	BODY( *lhs )
}
int
strncasecmp (const char *lhs, const char *rhs, size_t n)
{
	const char *end = &lhs[ zmin(n, strlen(lhs)) ];
	BODY( lhs < end )
}
# endif/* J__need_strcasecmp */

int
memcasecmp (const void *lhs, const void *rhs, size_t n)
{
	const void *end = ((const unsigned char **)&lhs)[ n ];
	BODY( lhs < end )
}
