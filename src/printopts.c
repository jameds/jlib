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

#include <stdio.h>
#include <string.h>

static const char *OPTION_SPACING = "  ";
int printopts (FILE *stream, const struct option options[], int n)
{
	int align;
	int indentlong;

	align = 0;
	indentlong = 0;

	int i;
	for (i = 0; i < n; i++)
	{
		int len;

		len = 0;
		if (options[i].option)
		{
			len += 2;
			indentlong = 1;
		}
		if (options[i].long_option)
		{
			if (options[i].option)
				len += 2;
			else
				if (indentlong)
					len += 4;

			len += 2 + strlen(options[i].long_option);
		}

		if (len)
		{
			if (options[i].argument)
				len += 1 + strlen(options[i].argument);

			/* todo: 80 column limit for all text? */

			if (len > align)
				align = len;
		}
		else
			indentlong = 0;
	}

	int nbytesw;

	nbytesw = 0;
	indentlong = 0;
	for (i = 0; i < n; i++)
	{
		int len;

#define EW(s, ...) fprintf(stream, s, ##__VA_ARGS__)
		nbytesw = EW(OPTION_SPACING);  /* indent */

		len = 0;
		if (options[i].option)
		{
			len += EW("-%c", options[i].option);
			indentlong = 1;
		}

		if (options[i].long_option)
		{
			if (options[i].option)
				len += EW(", ");
			else
				if (indentlong)
					len += EW("    ");

			len += EW("--%s", options[i].long_option);
		}

		if (len)
		{
			if (options[i].argument)
				len += EW("=%s", options[i].argument);

			nbytesw = EW("%*s%s%s", align - len, "", OPTION_SPACING,
							 options[i].description);
		}
		else
			indentlong = 0;

		putc('\n', stderr);
		nbytesw += len + 1;
#undef EW
	}

	return nbytesw;
}

/* vim: set ts=3 sw=3 ft=c :*/
