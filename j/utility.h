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

#ifndef JLIB_UTILITY_H
#define JLIB_UTILITY_H

#include <stdlib.h>  /* size_t */

#define MIN( a, b ) ( ((a) > (b)) ? (b) : (a) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

#define  min( a, b )  lmin (a, b)
#define  max( a, b )  lmax (a, b)
#define umin( a, b ) ulmin (a, b)
#define umax( a, b ) ulmax (a, b)

         long  lmin             (long a,          long b);
         long  lmax             (long a,          long b);
unsigned long ulmin    (unsigned long a, unsigned long b);
unsigned long ulmax    (unsigned long a, unsigned long b);
       size_t  zmin           (size_t a,        size_t b);
       size_t  zmax           (size_t a,        size_t b);

#define  around( x, y )  laround (x, y)
#define  afloor( x, y )  lafloor (x, y)
#define  aceil(  x, y )  laceil  (x, y)

         long  laround          (long x,          long y);
         long  lafloor          (long x,          long y);
         long  laceil           (long x,          long y);

#define uaround( x, y ) ularound (x, y)
#define uafloor( x, y ) ulafloor (x, y)
#define uaceil(  x, y ) ulaceil  (x, y)

unsigned long ularound (unsigned long x, unsigned long y);
unsigned long ulafloor (unsigned long x, unsigned long y);
unsigned long ulaceil  (unsigned long x, unsigned long y);

       double  faround        (double x,        double y);
       double  fafloor        (double x,        double y);
       double  faceil         (double x,        double y);

        float  faroundf        (float x,         float y);
        float  fafloorf        (float x,         float y);
        float  faceilf         (float x,         float y);

#endif
