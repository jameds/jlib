/* vim: set ts=3 sw=3 :*/
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

#include <j/utility.h>  /* max() */
#include <j/getopt.h>
#include <j/string.h>

#include <ctype.h>
#include <string.h>
#include <limits.h>

#define OVA_SHRT 1
#define OVA_LONG 2
#define OVA_BOTH 3

int j_getopt_flags = 0;

const char *j_printopts_leftmargin = "  ";
const char *j_printopts_halfway    = " ";

char *optstr;
char *optarg;
int   optind;

int (*j_printopts_function)(FILE *, struct j_option) = &j_printoption;

int j_printopts_all_varieties, j_printopts_varieties;
int j_printopts_indent;

static
int shiftback (int argc, char **argv, int n)
{
	char **argv_end;

	argc -= n;
	argv_end = argv + argc;

	for (; argv < argv_end; ++argv)
		*argv = argv[n];

	return n;
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

static
int getoptionnumber (int c, const struct j_option *options)
{
	int i;
	for (i = c-1; i >= 0; --i)
	{
		/* discount spacers */
		if (!options[i].short_option && options[i].long_option == NULL)
			c--;
	}
	return c;
}

static
int findoption (const char *s, int shorto, int aproximate,
		const struct j_option *options, int number_of_options)
{
	int (*compare)(const char *, const char *);

	int i;

	if (aproximate)
	{
		compare = ( (j_getopt_flags & J_GETOPT_CASE_INSENSITIVE) ?
				&strcaseaprox : &straprox );
	}
	else
	{
		compare = ( (j_getopt_flags & J_GETOPT_CASE_INSENSITIVE) ?
				&strcasecmp : &strcmp );
	}

	for (i = 0; i <number_of_options; ++i)
	{
		if (shorto)
		{
			if (*s == options[i].short_option)
				return i;
		}
		else
		{
			if (options[i].long_option)
			{
				if ((*compare)(s, options[i].long_option) == 0)
					return i;
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
int getlongest (const struct j_option *options, int number_of_options)
{
	int shortos = 0;
	int record = 0;
	int i, n;
	for (i = 0; i < number_of_options; ++i)
	{
		if (options[i].short_option && shortos < 2)
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

static
int getlongest_corrected (const struct j_option *options,int number_of_options,
		int varieties)
{
	/* getlongest() assumes that a  long option will always start with two '-'.
		However,without J_GETOPT_CONSISTENT_HYPHENS, j_printoption() will denote
		a long option with a single '-'  if no short options exist in the array.
	 */
	return ( (!(j_getopt_flags & J_GETOPT_CONSISTENT_HYPHENS) &&
				varieties == OVA_LONG) ?
			getlongest(options, number_of_options) - 1 :
			getlongest(options, number_of_options) );
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
						shiftback(*argcp - argi, &argv[argi], 1);
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
						if (( optind = findoption(argv[argi]++, 1, 0,
										options, number_of_options) ) > -1)
						{
							if (*argv[argi] == '\0')
								goto try_nextarg;
						}
					}
				}
				else
					if (( optind = findoption_certain(argv[argi], 0,
									options, number_of_options) ) > -1)
					{
					try_nextarg:
						if (options[optind].argument &&
								!(j_getopt_flags & J_GETOPT_SINGULAR) && (!pending
									|| !(j_getopt_flags & J_GETOPT_SHORT_SINGULAR)) &&
								argi + 1 < *argcp)
						{
							nextarg = argv[argi + 1];
						}
					}

				*argcp -=
					shiftback(*argcp - argi, &argv[argi], ( (nextarg) ? 2 : 1 ) );

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

	if (( optind = findoption_certain(optstr_save++, pending,
					options, number_of_options) ) > -1 &&
			options[optind].argument)
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
			if (optind == -1)  /* Not a valid choice. */
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

	return ( (optind == -1) ?
			number_of_options : getoptionnumber(optind, options) );
}

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

int j_printoption (FILE *stream, struct j_option option)
{
	int chars_wrote = 0, lm_chars_wrote;

#define PRINTF( ... ) chars_wrote += max(fprintf(stream, __VA_ARGS__), 0)

	if (!option.short_option && option.long_option == NULL)
		/* The spacer option is printed as simply a blank line. */
		return ( (putc('\n', stream) >= 0) ? 1 : 0 );

	lm_chars_wrote = max(fprintf(stream, "%s", j_printopts_leftmargin), 0);

	if (j_printopts_varieties == OVA_LONG)
	{
		PRINTF(( (j_getopt_flags & J_GETOPT_CONSISTENT_HYPHENS ||
						j_printopts_all_varieties == OVA_BOTH) ?
					"--%s" : "-%s" ),  option.long_option);
	}
	else
	{
		if (option.short_option)
			PRINTF("-%c", option.short_option);

		if (option.long_option)
		{
			PRINTF(( (option.short_option) ?
						", --%s" : "    --%s" ), option.long_option);
		}
	}

	if (option.argument)
	{
		chars_wrote += printargument(stream,
				option.argument, option.short_option);
	}

	if (chars_wrote < j_printopts_indent)
		PRINTF("%*s", j_printopts_indent - chars_wrote, " ");
	PRINTF("%s",  j_printopts_halfway);

	PRINTF("%s\n", option.description);

	return chars_wrote + lm_chars_wrote;
}

int j_printopts (FILE *stream,
		const struct j_option options[], int number_of_options,
		int start_segment, int segments)
{
	int bytes_wrote = 0;
	int i, u;

	j_printopts_all_varieties = getvarieties(options, number_of_options);
	if (j_getopt_flags & J_PRINTOPTS_ALIGN_ACROSS)
	{
		j_printopts_varieties = j_printopts_all_varieties;
		j_printopts_indent    = getlongest_corrected(options, number_of_options,
				j_printopts_all_varieties);
	}

	for (i = 0; start_segment > 0; --start_segment)
		i += findnulloption(&options[i], number_of_options - i) + 1;

	for (; i < number_of_options ;)
	{
		u = findnulloption(&options[i], number_of_options - i);

		if (!(j_getopt_flags & J_PRINTOPTS_ALIGN_ACROSS))
		{
			j_printopts_varieties = getvarieties(&options[i], u);
			j_printopts_indent = getlongest_corrected(&options[i], u,
					j_printopts_all_varieties);
		}

		u += i;
		if (segments == 0 || --segments > 0)
		{
			if (u != number_of_options)
				u++;

			for (; i < u; ++i)
				bytes_wrote += (*j_printopts_function)(stream, options[i]);
		}
		else
		{
			for (; i < u; ++i)
				bytes_wrote += (*j_printopts_function)(stream, options[i]);

			break;
		}
	}

	return bytes_wrote;
}
