/* vim: set ts=3 sw=3 ft=c :*/
/*
Copyright 2018 by James R.

This file is a part of jlib.

jlib is free software; you can redistribute it and/or modify
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

#include <j/getopt.h>

#include <ctype.h>
#include <string.h>
#include <limits.h>

#define OVA_SHRT 1
#define OVA_LONG 2
#define OVA_BOTH 3

int j_getopt_flags = 0;

char *optstr;
char *optarg;

static
int shiftback (int *argcp, char **argv, int n)
{
	char **argv_end;

	(*argcp) -= n;
	argv_end = argv + *argcp;

	for (; argv < argv_end; ++argv)
		*argv = argv[n];

	return *argcp;
}

static
int strcmp_insensitive (lhs, rhs)
	const char *lhs;
	const char *rhs;
{
	for (;; ++lhs, ++rhs)
	{
		if (toupper(*lhs) != toupper(*rhs) || *lhs == '\0')
			break;
	}
	return *lhs - *rhs;
}

static
int findoption (const char *s, int shorto, int aproximate,
		const struct j_option *options, int number_of_options)
{
	int (*compare)(const char *, const char *) =
		( (j_getopt_flags & J_GETOPT_CASE_INSENSITIVE) ?
				&strcmp_insensitive : &strcmp );

	size_t final = strlen(s)-1;
	int c;
	int spacers = 0;
	int i;

	for (i = 0; i <number_of_options; ++i)
	{
		if (!options[i].short_option && options[i].long_option == NULL)
			spacers++;
		else
		if (shorto)
		{
			if (*s == options[i].short_option)
				return i - spacers;
		}
		else
		{
			if (options[i].long_option)
			{
				c = (*compare)(s, options[i].long_option);
				if (c == 0 || (c < 0 && aproximate &&
							(strlen(options[i].long_option)-1 > final &&
								options[i].long_option[final] == s[final])))
				{
					return i - spacers;
				}
			}
		}
	}
	return -1;
}

static
int findoption_certain (const char *s, int shorto,
		const struct j_option *options, int number_of_options)
{
	int i = findoption(s, shorto, 0, options, number_of_options);
	if (j_getopt_flags & J_GETOPT_APROXIMATE && i == -1 && !shorto)
		return findoption(s, shorto, 1, options, number_of_options);
	else
		return i;
}

static
int findnulloption (const struct j_option *options, int number_of_options)
{
	int i;
	for (i = 0; i < number_of_options; ++i)
	{
		if (!options[i].short_option && options[i].long_option == NULL)
			break;
	}
	return i;
}

static
int getvarieties (const struct j_option *options, int number_of_options)
{
	int varie = 0;
	int i;
	for (i = 0; i < number_of_options; ++i)
	{
		     if (options[i].short_option) varie |= OVA_SHRT;
		else if  (options[i].long_option) varie |= OVA_LONG;

		if (varie == OVA_BOTH)  /* We've already got what we want to know. */
			break;
	}
	return varie;
}

int j_getopt (int *argcp, char *argv[],
		const struct j_option options[], int number_of_options)
{
	static char **argv_cache;

	/* After a call to shiftback(), the current option can no longer be viewed
		via argv.  This doesn't work for short option strings, which must be
		queued over multiple calls to j_getopt().Therefore, a call to shiftback()
		cannot be preformed until the short option string terminates.  However,
		this presents another problem: The shiftback() usage was implemented to
		allow the user to view argv with the previously parsed options and their
		arguments removed.  Therefore, we save the current option string so that
		we can access it after calling shiftback().
	 */
	static char *optstr_save;  /* Including the rest of the string. */
	static char *nextarg;      /* We can remove an argument early.  */

	static int  pending = 0;  /* Short options are pending. */
	static char pending_cstr[3] = { '-', 0, '\0' };

	int only_long_options;

	static int argi;
	int i;

	if (argv != argv_cache)
	{
		argv_cache = argv;
		argi = 1;
	}

	if (argv == NULL)
		return -1;

	only_long_options = (getvarieties(options, number_of_options) == OVA_LONG);

	if (!pending)
	{
		for (;;)
		{
			if (argi >= *argcp)  /* We've reached the end. */
				return -1;

			optstr = argv[argi];  /* Including the hyphens. */

			/* A single hyphen is treated as a regular argument. */
			if (argv[argi][0] != '-' || argv[argi][1] == '\0')
			{
				if (j_getopt_flags & J_GETOPT_OPTIONS_FIRST)
					return -1;
				argi++;
			}
			else
			{
				argv[argi]++;

				if (*argv[argi] == '-')  /* Could be a long option. */
				{
					argv[argi]++;
					/* A double hyphen is considered to halt the options. */
					if (*argv[argi] == '\0')
					{
						shiftback(argcp, &argv[argi], 1);
						return -1;
					}
					else
						optstr_save = argv[argi];
				}
				else
				{
					optstr_save = argv[argi];

					if (j_getopt_flags & J_GETOPT_CONSISTENT_HYPHENS ||
							!only_long_options)
					{
						pending = 1;
					}
				}

				nextarg = NULL;
				if (pending)
				{
					/* Artifically parse the short option string. */
					while (*argv[argi])
					{
						if (( i = findoption(argv[argi]++, 1, 0,
										options, number_of_options) ) > -1)
						{
							if (*argv[argi] == '\0')
								goto try_nextarg;
						}
					}
				}
				else
					if (( i = findoption_certain(argv[argi], 0,
									options, number_of_options) ) > -1)
					{
					try_nextarg:
						if (options[i].argument &&
								!(j_getopt_flags & J_GETOPT_SINGULAR) && (!pending
									|| !(j_getopt_flags & J_GETOPT_SHORT_SINGULAR)) &&
								argi + 1 < *argcp)
						{
							nextarg = argv[argi + 1];
						}
					}

				shiftback(argcp, &argv[argi], ( (nextarg) ? 2 : 1 ));

				break;
			}
		}
	}

	if (pending)
	{
		pending_cstr[1] = *optstr_save;
		optstr = pending_cstr;

		if (optstr_save[1])  /* Argument directly after the character. */
		{
			optarg = &optstr_save[1];
			if (j_getopt_flags & J_GETOPT_ALWAYS_DELIMIT && *optarg == '=')
				*optarg++ = '\0';
		}
	}
	else
	{
		if (( optarg = strchr(optstr_save, '=') ))
			*optarg++ = '\0';
	}

	if (( i = findoption_certain(optstr_save++, pending,
					options, number_of_options) ) > -1 &&
			options[i].argument)
	{
		if ((!(j_getopt_flags & J_GETOPT_SINGULAR) && (!pending ||
						!(j_getopt_flags & J_GETOPT_SHORT_SINGULAR))) &&
				(!optarg && nextarg))
		{
			optarg = nextarg;
		}

		pending = 0;
	}
	else
	{
		if (!(j_getopt_flags & J_GETOPT_PASSIVE))
		{
			if (i == -1)  /* Not a valid choice. */
			{
				fprintf(stderr, "``%s'' is not an available option.\n", optstr);
				return -1;
			}
			else          /* Illegally passed argument. */
			if (optarg && (!pending || j_getopt_flags & J_GETOPT_ALWAYS_DELIMIT))
			{
				fprintf(stderr, "``%s'' does not accept an argument.\n", optstr);
				return -1;
			}
		}

		/* By using an '=', an argument could be forcibly added.
			But that behavior does not have much merit, so it's not supported. */
		optarg = NULL;

		if (pending && *optstr_save == '\0')  /* Short option string ends. */
			pending = 0;
	}

	return ( (i == -1) ?number_of_options : i );
}

static int
max (a, b)
	int a;
	int b;
{
	return ( (a > b) ? a : b );
}

const char *j_printopts_leftmargin = "  ";

static
int printargument (FILE *stream, const char *argument, int shorto)
{
	if (j_getopt_flags & J_GETOPT_ALWAYS_DELIMIT)
	{
		return fprintf(stream, "=%s", argument);
	}
	else
	if (shorto)
	{
		return fprintf(stream,
				( (j_getopt_flags & (J_GETOPT_SINGULAR | J_GETOPT_SHORT_SINGULAR))?
					"%s" : " %s" ), argument);
	}
	else
	{
		return fprintf(stream, ( (j_getopt_flags & J_GETOPT_SINGULAR) ?
					"=%s" : " %s" ), argument);
	}
}

static
int getlongest (const struct j_option *options, int number_of_options)
{
	int shortos = 0;
	int record = 0;
	int i, n;
	for (i = 0; i < number_of_options; ++i)
	{
		if (options[i].short_option && shortos < 1)
			shortos = 2;

		if (options[i].long_option)
		{
			if (shortos == 2)
				shortos = 4;
			n = 2 + strlen(options[i].long_option) + 1;
		}
		else
		{
			n = ( (j_getopt_flags & J_GETOPT_ALWAYS_DELIMIT ||
						!(j_getopt_flags &
							(J_GETOPT_SINGULAR | J_GETOPT_SHORT_SINGULAR))) ?
					1 : 0 );
		}

		n += ( (options[i].argument) ? strlen(options[i].argument) : -1 );

		if (n > record)
			record = n;
	}
	return record + shortos;
}

static int
getlongest_corrected (const struct j_option *options, int number_of_options,
		int varieties)
{
	/* getlongest() assumes that a long option will always start with two '-'.
		However, without J_GETOPT_CONSISTENT_HYPHENS, j_getopt() accepts a long
		option denoted with a single '-' if no short options exist in the array.
	 */
	return ( (j_getopt_flags & J_GETOPT_CONSISTENT_HYPHENS &&
				varieties != OVA_LONG) ?
			getlongest(options, number_of_options) + 1 :
			getlongest(options, number_of_options) );
}

int j_printopts (FILE *stream,
		const struct j_option options[], int number_of_options)
{
	int bytes_wrote = 0, n;

	int varieties;
	int width;

	int i, u;

	varieties = getvarieties(options, number_of_options);

	if (j_getopt_flags & J_PRINTOPTS_ALIGN_ACROSS)
		width = getlongest_corrected(options, number_of_options, varieties);

	for (i = 0; i < number_of_options; ++i)
	{
		u = findnulloption(&options[i], number_of_options - i);

		if (!(j_getopt_flags & J_PRINTOPTS_ALIGN_ACROSS))
			width = getlongest_corrected(&options[i], u, varieties);

		for (u += i; i < u; ++i)
		{
			bytes_wrote += max(fprintf(stream, "%s", j_printopts_leftmargin), 0);

			n = 0;
#define PRINTF( ... ) n += max(fprintf(stream, __VA_ARGS__), 0)
			if (varieties == OVA_LONG)
			{
				PRINTF(( (j_getopt_flags & J_GETOPT_CONSISTENT_HYPHENS) ?
							"--%s" : "-%s" ),  options[i].long_option);
			}
			else
			{
				if (options[i].short_option)
					PRINTF("-%c", options[i].short_option);

				if (options[i].long_option)
				{
					PRINTF(( (options[i].short_option) ?
								", --%s" : "    --%s" ), options[i].long_option);
				}
			}

			if (options[i].argument)
			{
				n += printargument(stream,
						options[i].argument, options[i].short_option);
			}

			PRINTF("%*s", width - n, " ");

			PRINTF("%s\n", options[i].description);

			bytes_wrote += n;
		}

		if (i != number_of_options)
		{
			putc('\n', stream);
			bytes_wrote++;
		}
	}

	return bytes_wrote;
}
