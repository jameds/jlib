/*
	Copyright 2018 by James R.

	This file is part of jlib.
	And jlib, in the following text, shall be refered to as "this library".

	This library is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This library is distributed om the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JLIB_GETOPT_H
#define JLIB_GETOPT_H

#include <stdio.h>

struct option
{
	char  option;
	char *long_option;
	char *argument;

	char *description;
};

extern char *optstr;  /* The option string; does not include an '='. */
extern char *optarg;  /* The option argument or NULL.                */

/*
	Option strings may be prefixed with a single '-' for single character
	options, or a double '-' for string options (GNU long options).
	A single '-' may be used for string options if no single character options
	are defined.

	Option arguments may be stored as a seperate array element or as part
	of the option string, delimited by a '='.  It may also be written directly
	after a single character option.

	The index of a matched option is returned.
	If an unrecognized option is encountered, the number of options  is returned
	Upon reaching the end of argv or terminating option (--), -1 is returned.

	Do not call this function with different parameters without
	prior calling resetgetopte().
 */
int  getopte   (int *argc, char *argv[], const struct option options[], int n);
void resetgetopte (void);
int  printopts    (FILE *stream, const struct option options[], int n);

#endif
