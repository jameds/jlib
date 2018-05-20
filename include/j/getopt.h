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

#ifndef JLIB_GETOPT_H
#define JLIB_GETOPT_H

#include <j/version.h>

#include <stdio.h>

#define J_GETOPT_ALWAYS_DELIMIT     1 /* Always permit the use of a '=' to   *
                                       * delimit an option's argument.       */
#define J_GETOPT_CONSISTENT_HYPHENS 2 /* Do not accept a single hyphen to de-*
                                       * limit a long option when no short   *
                                       * options are defined.                */
#define J_GETOPT_OPTIONS_FIRST      4 /* Halt option parsing when the first  *
                                       * regular argument is encountered.    */
#define J_GETOPT_PASSIVE            8 /* Do not preform error checking. This *
                                       * flag is recommended.                */
#define J_GETOPT_SINGULAR          16 /* Option arguments must be present in *
                                       * the same argument as the option.    */
#define J_GETOPT_SHORT_SINGULAR    32 /* A short option's argument must be   *
                                       * present in the same argument as the *
                                       * option.                             */
#define J_GETOPT_APROXIMATE        64 /* Match a long option even partially. */
#define J_GETOPT_CASE_INSENSITIVE 128 /* Long options are matched case insen-*
                                       * sitively                            */
#define J_PRINTOPTS_ALIGN_ACROSS  256 /* Align even across spacer options.   */

/*
short_option may be 0; long_option and argument may be NULL.   description must
always be a valid string, unless the option which it belongs to is a spacer op-
tion.

If short_option is 0  and long_option is NULL,  that option shall be considered
a "spacer" option.  A spacer option only makes visual difference (with j_print-
option()). A spacer option  shall be printed as a blank line.   Unless J_PRINT-
OPTS_ALIGN_ACROSS is in effect,  the segments of options between spacer options
shall be indented separately.

Although J_PRINTOPTS_ALIGN_ACROSS contains the name "PRINTOPTS",  it must still
be set with j_getopt_flags.
 */
struct j_option
{
	char short_option;
	char *long_option;
	char *argument;

	char *description;
};

extern int j_getopt_flags;

/*
A string to print before all options, via j_printopts().  It is not printed for
spacer options.  The string is initialized with two spaces for indenting.
*/
extern const char *j_printopts_leftmargin;
/*
A string to print before the description, via j_printopts(). The string is ini-
tialized with a single space.
*/
extern const char *j_printopts_halfway;

/*
A string containing the characters of the current option. For long options, the
string contains the option just as it was passed via the argument vector.   For
short options,the string contains merely the option's character, prepended with
a '-'.
 */
extern char *optstr;
extern char *optarg; /* The current option's argument or NULL. */

/*
A pointer to the function which j_printopts() should call to print each option.
Such printing may include the left margin, also.The function pointer is initia-
lized to j_printoption().     The function must return the number of characters
that were written to the given stream.
*/
extern int (*j_printopts_function)(FILE *stream, struct j_option option);

/*
The length of the longest option in the current segment as defined by j_print-
option().
*/
extern int j_printopts_width;
/*
The variety of options present in the options array passed to j_printopts(),and
the current segment.Valid values are 1, for short options, 2, for long options,
and 3, for both short and long options.
*/
extern int j_printopts_all_varieties, j_printopts_varieties;

/*
j_getopt()  shall search for and parse options,  denoted  by an initial
'-',  in the argument vector  argv.   These options are  defined  in an
options array.   A single '-' with  nothing following  is  considered a
regular argument.   Upon parsing an option, the arguments following the
argument of which the option was  found shall be permuted so that those
arguments are shifted back,  overwriting the parsed option and possibly
its  argument.   The argument  count pointed to by  argcp shall also be
decreased to reflect this permutation.

If an option was parsed to be of the options array,  j_getopt() returns
the index of that option within the array,  discounting spacer options.
If the option is not valid in the array,   j_getopt() may err or return
a value larger than the final index of the array. When further argument
parsing is prevented, j_getopt() returns a negative value.

Two varieties  of options  are recognized,  "short" and "long".   Short
options are identified by a single character.This is commonly a letter.
Long  options  are denoted  by an  extra '-'  followed  by a  string of
characters not containing white space.  If no short options are defined
in the options array, the extraneous '-' becomes optional.

Additionally,  "--" is a  special option  that delimits  the  preceding
options.  Upon encountering such an option, further argument parsing is
halted.

Options  may be  defined to accept an  argument.  Such  an argument  is
optional and  evaluating of it must be fufilled  by the program.   This
argument may be  written in the same argument as the option,  or in the
next argument.  For short options, the argument can be written directly
after  the option.  For  long options,  it can be  delimited  by a '='.
Explicitly passing an argument  via delimitation to an option that does
not accept  an argument is considered an error,  which may halt further
parsing.

The  usage  of  delimitation  may  be  extended  to  short  options  by
J_GETOPT_ALWAYS_DELIMIT.The optionality of an extra '-' denoting a long
option when there  are no  short options  defined may be  disabled with
J_GETOPT_CONSISTENT_HYPHENS.  When J_GETOPT_OPTIONS_FIRST is in effect,
upon encountering  an argument that is not an option,  parsing shall be
halted. J_GETOPT_PASSIVE may be used to disable the error reporting and
automatic halting provided by the function.   When J_GETOPT_SINGULAR or
J_GETOPT_SHORT_SINGULAR (applying only to short options) are in effect,
an option's argument may not be written  in the argument proceding that
option's argument.

j_getopt() internally tracks the given argument vector.This permits the
program to pass different vectors to the function.   If a re-evaluation
of  the same vector  is desired,  NULL may be  passed as  the vector to
reset the internal tracking.   All other arguments are interchangeable.
 */
int j_getopt    (int *argcp, char *argv[],
      const struct j_option options[], int number_of_options);

/*
j_printopts() shall print in consecutive order, on separate lines, the
usage and description of each option defined in the array options.Seg-
ments is the number of segments (see "struct j_option") which shall be
printed.   A value of 0 indicates that all segments should be printed.

Each option is printed by calling  the function pointed to by j_print-
opts_function.
*/
int j_printopts (FILE *stream,
      const struct j_option options[], int number_of_options,
      int start_segment, int segments);

/*
The format of the option shall be  determined according to paragraph 3,
5 and 6 of the documentation for j_getopt(). Syntax changeable by flags
is prefered.  Long options are indented if short options are present in
the current segment.   See "struct j_option", "`spacer' options".   The
description is indented to match the longest option and argument.
*/
int j_printoption (FILE *stream, struct j_option option);

#endif
