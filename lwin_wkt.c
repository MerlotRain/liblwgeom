/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "liblwgeom_internel.h"

#include "lwgeom_ordinate.h"
#include "stok.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

/* ----------------------------- static read wkt ---------------------------- */

static char *wkt_next_word(stok_t *token);
static double wkt_get_next_number(stok_t *token);
static void wkt_get_coordinates(stok_t *token, lwgeom_ordinate *flag, double **coordinates, int *num);
static void wkt_get_precise_coordinates(stok_t *token, lwgeom_ordinate *flag, double *coordinates);
static char *wkt_get_next_empty_or_opener(stok_t *token, lwgeom_ordinate *flag);
static char *wkt_get_next_closer_comma(stok_t *token);
static LWGEOM *wkt_read_point(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_linestring(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_linearring(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_polygon(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_multipoint(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_multilinestring(stok_t *token, lwgeom_ordinate *flag);
static LWGEOM *wkt_read_multipolygon(stok_t *token, lwgeom_ordinate *flag);

/* -------------------------------- input wkt ------------------------------- */

LWGEOM *
lwgeom_read_wkt(const char *data, size_t len)
{
	char *p = setlocale(LC_NUMERIC, NULL);
	setlocale(LC_NUMERIC, "C");

	stok_t token;
	stok_init(&token, (char *)data);

	lwgeom_ordinate flags = lwgeom_ordinate_XY();
	lwgeom_ordinate new_flags = lwgeom_ordinate_XY();
	char *type = wkt_next_word(&token);
	if (strcmp(type, "EMPTY") == 0)
	{
		memcpy(&new_flags, &flags, sizeof(lwgeom_ordinate));
	}
	else
	{
		if (strlen(type) >= 2 && 0 == strncmp(type + strlen(type) - 2, "ZM", 2))
		{
			lwgeom_ordinate_setZ(&new_flags, LW_TRUE);
			lwgeom_ordinate_setM(&new_flags, LW_TRUE);
			new_flags.changeAllowed = LW_FALSE;
		}
		else if (strlen(type) >= 1 && 0 == strncmp(type + strlen(type) - 1, "M", 1))
		{
			lwgeom_ordinate_setM(&new_flags, LW_TRUE);
			new_flags.changeAllowed = LW_FALSE;
		}
		else if (strlen(type) >= 1 && 0 == strncmp(type + strlen(type) - 1, "Z", 1))
		{
			lwgeom_ordinate_setZ(&new_flags, LW_TRUE);
			new_flags.changeAllowed = LW_FALSE;
		}
	}

	if (strncmp(type, "POINT", 5) == 0)
	{
		return wkt_read_point(&token, &new_flags);
	}
	else if (strncmp(type, "LINESTRING", 10) == 0)
	{
		return wkt_read_linestring(&token, &new_flags);
	}
	else if (strncmp(type, "LINEARRING ", 10) == 0)
	{
		return wkt_read_linearring(&token, &new_flags);
	}
	else if (strncmp(type, "POLYGON", 7) == 0)
	{
		return wkt_read_polygon(&token, &new_flags);
	}
	else if (strncmp(type, "MULTIPOINT", 10) == 0)
	{
		return wkt_read_multipoint(&token, &new_flags);
	}
	else if (strncmp(type, "MULTILINESTRING", 15) == 0)
	{
		return wkt_read_multilinestring(&token, &new_flags);
	}
	else if (strncmp(type, "MULTIPOLYGON", 12) == 0)
	{
		return wkt_read_multipolygon(&token, &new_flags);
	}
	// else if (strncmp(type, "CIRCULARSTRING", 14) == 0) {
	//     return NULL;
	// }
	// else if (strncmp(type, "COMPOUNDCURVE", 13) == 0) {
	//     return NULL;
	// }
	// else if (strncmp(type, "CURVEPOLYGON", 12) == 0) {
	//     return NULL;
	// }
	// else if (strncmp(type, "MULTICURVE", 10) == 0) {
	//     return NULL;
	// }
	// else if (strncmp(type, "MULTISURFACE", 12) == 0) {
	//     return NULL;
	// }
	// else if (strncmp(type, "GEOMETRYCOLLECTION", 18) == 0) {
	//     return NULL;
	// }

	setlocale(LC_NUMERIC, p);
	return NULL;
}

/* ----------------------------- static read wkt ---------------------------- */

char *
wkt_next_word(stok_t *token)
{
	int type = stok_next_token(token);
	switch (type)
	{
	case STOK_EOF:
	case STOK_EOL:
	case STOK_NUM:
		return "";
	case STOK_WORD: {
		char *word = token->stok;
		char *str = word;
		while (*str)
		{
			*str = (char)toupper((unsigned char)*str);
			str++;
		}
		return word;
	}
	case '(':
		return "(";
	case ')':
		return ")";
	case ',':
		return ",";
	};
	return "";
}

double
wkt_get_next_number(stok_t *token)
{
	int type = stok_next_token(token);
	switch (type)
	{
	case STOK_NUM:
		return token->ntok;
	default:
		return 0.0;
	}
	return 0;
}

void
wkt_get_coordinates(stok_t *token, lwgeom_ordinate *flag, double **coordinates, int *num)
{
	char *nexttok = wkt_get_next_empty_or_opener(token, flag);
	if (strcmp(nexttok, "EMPTY") == 0)
	{
		return;
	}
	double c[4] = {0.0};
	wkt_get_precise_coordinates(token, flag, c);

	// calloc coordinates with cdim
	int cdim = (flag->value & LWORDINATE_VALUE_Z) ? 3 : 2;
	*coordinates = (double *)lwcalloc(cdim, sizeof(double));
	if (*coordinates == NULL)
	{
		return;
	}
	*num = 1;
	memcpy(*coordinates, c, cdim * sizeof(double));

	nexttok = wkt_get_next_closer_comma(token);
	while (strcmp(nexttok, ")") != 0)
	{
		memset(c, 0, sizeof(c));
		wkt_get_precise_coordinates(token, flag, c);
		*coordinates = (double *)lwrealloc(*coordinates, (*num + 1) * sizeof(double) * (cdim));
		if (*coordinates == NULL)
		{
			lwfree(*coordinates);
			*num = 0;
			// log error
			return;
		}
		memcpy((*coordinates) + ((*num) * cdim), c, cdim * sizeof(double));
		(*num)++;
		nexttok = wkt_get_next_closer_comma(token);
	}
}

char *
wkt_get_next_empty_or_opener(stok_t *token, lwgeom_ordinate *flag)
{
	char *nextword = wkt_next_word(token);
	if (strcmp(nextword, "ZM") == 0)
	{
		nextword = wkt_next_word(token);
	}
	else
	{
		if (strcmp(nextword, "Z") == 0)
		{
			nextword = wkt_next_word(token);
		}
		if (strcmp(nextword, "M") == 0)
		{
			nextword = wkt_next_word(token);
		}
	}

	if (strcmp(nextword, "(") == 0 || strcmp(nextword, "EMPTY") == 0)
	{
		return nextword;
	}
	return "";
}

char *
wkt_get_next_closer_comma(stok_t *token)
{
	char *nextWord = wkt_next_word(token);
	if (strcmp(nextWord, ",") == 0 || strcmp(nextWord, ")") == 0)
	{
		return nextWord;
	}
	return NULL;
}

static void
wkt_get_precise_coordinates(stok_t *token, lwgeom_ordinate *flag, double *coordinates)
{
	coordinates[0] = wkt_get_next_number(token);
	coordinates[1] = wkt_get_next_number(token);

	// Check for undeclared Z dimension
	if (flag->changeAllowed && (stok_peek_next_token(token) == STOK_NUM))
		lwgeom_ordinate_setZ(flag, LW_TRUE);

	if (flag->value & LWORDINATE_VALUE_Z)
		coordinates[2] = wkt_get_next_number(token);

	// Check for undeclared M dimension
	if (flag->changeAllowed && (flag->value & LWORDINATE_VALUE_Z) && (stok_peek_next_token(token) == STOK_NUM))
		lwgeom_ordinate_setM(flag, LW_TRUE);

	if (flag->value & LWORDINATE_VALUE_M)
		coordinates[3] = wkt_get_next_number(token);

	flag->changeAllowed = LW_FALSE;
}

LWGEOM *
wkt_read_point(stok_t *token, lwgeom_ordinate *flag)
{
	double *coord = NULL;
	int n = 0;
	wkt_get_coordinates(token, flag, &coord, &n);
	if (coord && n == 1)
	{
	}
	return NULL;
}

LWGEOM *
wkt_read_linestring(stok_t *token, lwgeom_ordinate *flag)
{
	double *coord = NULL;
	int n = 0;
	wkt_get_coordinates(token, flag, &coord, &n);
	if (coord && n > 1)
	{
	}
	return NULL;
}

LWGEOM *
wkt_read_linearring(stok_t *token, lwgeom_ordinate *flag)
{
	double *coord = NULL;
	int n = 0;
	wkt_get_coordinates(token, flag, &coord, &n);
	if (coord && n > 1)
	{
	}
	return NULL;
}

LWGEOM *
wkt_read_polygon(stok_t *token, lwgeom_ordinate *flag)
{
	char *nextToken = wkt_get_next_empty_or_opener(token, flag);
	if (strncmp(nextToken, "EMPTY", 5) == 0)
		return NULL;

	LWGEOM **subs = (LWGEOM **)lwcalloc(1, sizeof(LWGEOM *));
	if (subs == NULL)
		return NULL;
	LWGEOM *shell = wkt_read_linearring(token, flag);
	subs[0] = shell;
	nextToken = wkt_get_next_closer_comma(token);
	while (strcmp(nextToken, ",") == 0) {}

	return NULL;
}

LWGEOM *
wkt_read_multipoint(stok_t *token, lwgeom_ordinate *flag)
{
	return NULL;
}

LWGEOM *
wkt_read_multilinestring(stok_t *token, lwgeom_ordinate *flag)
{
	return NULL;
}

LWGEOM *
wkt_read_multipolygon(stok_t *token, lwgeom_ordinate *flag)
{
	return NULL;
}