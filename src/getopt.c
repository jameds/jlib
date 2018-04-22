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

#include <string.h>

static inline char * strchrnul (const char *s, int c)
{
	char *p;
	if ((p = strchr(s, c)) == NULL)
		return (char *) s + strlen(s);
	return p;
}

char *optstr;
char *optarg;

static int ac;
int getopte (int *argc, char *argv[], const struct option options[], int n)
{
	static int pending = 0;  /* short option string */
	static int xopt    = 1;  /* only long options? */
	static int ai;

	int   opti;
	char *adp;  /* last character before argument. */

	if (ac == 0)
	{
		ac    = *argc;
		*argc = 1;

		for (opti = 0; opti < n; opti++)
			if (options[opti].option)
			{
				xopt = 0;
				break;
			}
	}

	optarg = NULL;

	if (!pending)
	{
		while (++ai < ac)
			/* A single dash is treated as a regular argument. */
			if (*argv[ai] != '-' || *++argv[ai] == 0)
				argv[(*argc)++] = argv[ai];
			else
			{
				if (*argv[ai] == '-')  /* long option */
				{
					if (*++argv[ai] == 0)  /* end of options */
					{
						while (++ai < ac)
							argv[(*argc)++] = argv[ai];

						return -1;
					}
				}
				else
					if (!xopt)
						pending = 1;
				break;
			}

		if (ai >= ac)
			return -1;
	}

	optstr = argv[ai];

	if (pending)
	{
		adp = argv[ai];
		if (*(adp + 1) == '=')
			*++adp = 0;
	}
	else
	{
		adp = strchrnul(optstr, '=');
		if (*adp)
			*adp = 0;
		else
			adp--;
	}

	for (opti = 0; opti < n; opti++)
		if (pending)  /* (next) short option */
		{
			if (*optstr == options[opti].option)
				break;
		}
		else
			if (options[opti].long_option)
			{
				if (strcmp(optstr, options[opti].long_option) == 0)
					break;
			}

	if (opti < n)
		if (options[opti].argument)
		{
			argv[ai] = adp + 1;

			/* -oarg, --o arg, --o=arg */
			if (!*adp || *argv[ai] || ++ai != ac)
				optarg = argv[ai];

			pending = 0;  /* break option */
			return opti;
		}
	if (*adp && *++argv[ai] == 0)  /* end short option string */
		pending = 0;

	return opti;
}

void resetgetopte (void)
{
	ac = 0;
}

/* vim: set ts=3 sw=3 ft=c :*/
