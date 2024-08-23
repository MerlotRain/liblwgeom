/*****************************************************************************/
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "nvp.h"

#include "ordinate.h"
#include "stok.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/* ----------------------------- static read wkt ---------------------------- */

static char *_inwkt_next_word(stok *token);

static double _inwkt_get_next_number(stok *token);

static void _inwkt_get_coordinates(stok *token, struct Ordinate *flag,
                                   double **coordinates, int *num);

static void _inwkt_get_precise_coordinates(stok *token, struct Ordinate *flag,
                                           double *coordinates);

static char *_inwkt_get_next_empty_or_opener(stok *token,
                                             struct Ordinate *flag);

static char *_inwkt_get_next_closer_comma(stok *token);

static struct nv_geobject *_inwkt_read_point(stok *token,
                                             struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_linestring(stok *token,
                                                  struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_linearring(stok *token,
                                                  struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_polygon(stok *token,
                                               struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_multipoint(stok *token,
                                                  struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_multilinestring(stok *token,
                                                       struct Ordinate *flag);

static struct nv_geobject *_inwkt_read_multipolygon(stok *token,
                                                    struct Ordinate *flag);

/* -------------------------------- inner wkt ------------------------------- */

struct nv_geobject *mg_read_wkt(const char *data, size_t len)
{
    char *p = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "C");

    stok token;
    stok_init(&token, (char *)data);

    struct Ordinate flags = ordinate_XY();
    struct Ordinate new_flags = ordinate_XY();
    char *type = _inwkt_next_word(&token);
    if (strcmp(type, "EMPTY") == 0) {
        memcpy(&new_flags, &flags, sizeof(struct Ordinate));
    }
    else {
        if (strlen(type) >= 2 &&
            0 == strncmp(type + strlen(type) - 2, "ZM", 2)) {
            ordinate_setZ(&new_flags, true);
            ordinate_setM(&new_flags, true);
            new_flags.changeAllowed = false;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "M", 1)) {
            ordinate_setM(&new_flags, true);
            new_flags.changeAllowed = false;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "Z", 1)) {
            ordinate_setZ(&new_flags, true);
            new_flags.changeAllowed = false;
        }
    }

    if (strncmp(type, "POINT", 5) == 0) {
        return _inwkt_read_point(&token, &new_flags);
    }
    else if (strncmp(type, "LINESTRING", 10) == 0) {
        return _inwkt_read_linestring(&token, &new_flags);
    }
    else if (strncmp(type, "LINEARRING ", 10) == 0) {
        return _inwkt_read_linearring(&token, &new_flags);
    }
    else if (strncmp(type, "POLYGON", 7) == 0) {
        return _inwkt_read_polygon(&token, &new_flags);
    }
    else if (strncmp(type, "MULTIPOINT", 10) == 0) {
        return _inwkt_read_multipoint(&token, &new_flags);
    }
    else if (strncmp(type, "MULTILINESTRING", 15) == 0) {
        return _inwkt_read_multilinestring(&token, &new_flags);
    }
    else if (strncmp(type, "MULTIPOLYGON", 12) == 0) {
        return _inwkt_read_multipolygon(&token, &new_flags);
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

char *_inwkt_next_word(stok *token)
{
    int type = stok_next_token(token);
    switch (type) {
    case STOK_EOF:
    case STOK_EOL:
    case STOK_NUM:
        return "";
    case STOK_WORD: {
        char *word = token->stok;
        char *str = word;
        while (*str) {
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

double _inwkt_get_next_number(stok *token)
{
    int type = stok_next_token(token);
    switch (type) {
    case STOK_NUM:
        return token->ntok;
    default:
        return 0.0;
    }
    return 0;
}

void _inwkt_get_coordinates(stok *token, struct Ordinate *flag,
                            double **coordinates, int *num)
{
    char *nexttok = _inwkt_get_next_empty_or_opener(token, flag);
    if (strcmp(nexttok, "EMPTY") == 0) {
        return;
    }
    double c[4] = {0.0};
    _inwkt_get_precise_coordinates(token, flag, c);

    // calloc coordinates with cdim
    int cdim = (flag->value & ORDINATE_VALUE_Z) ? 3 : 2;
    *coordinates = (double *)calloc(cdim, sizeof(double));
    if (*coordinates == NULL) {
        return;
    }
    *num = 1;
    memcpy(*coordinates, c, cdim * sizeof(double));

    nexttok = _inwkt_get_next_closer_comma(token);
    while (strcmp(nexttok, ")") != 0) {
        memset(c, 0, sizeof(c));
        _inwkt_get_precise_coordinates(token, flag, c);
        *coordinates = (double *)realloc(*coordinates,
                                         (*num + 1) * sizeof(double) * (cdim));
        if (*coordinates == NULL) {
            free(*coordinates);
            *num = 0;
            // log error
            return;
        }
        memcpy((*coordinates) + ((*num) * cdim), c, cdim * sizeof(double));
        (*num)++;
        nexttok = _inwkt_get_next_closer_comma(token);
    }
}

char *_inwkt_get_next_empty_or_opener(stok *token, struct Ordinate *flag)
{
    char *nextword = _inwkt_next_word(token);
    if (strcmp(nextword, "ZM") == 0) {
        nextword = _inwkt_next_word(token);
    }
    else {
        if (strcmp(nextword, "Z") == 0) {
            nextword = _inwkt_next_word(token);
        }
        if (strcmp(nextword, "M") == 0) {
            nextword = _inwkt_next_word(token);
        }
    }

    if (strcmp(nextword, "(") == 0 || strcmp(nextword, "EMPTY") == 0) {
        return nextword;
    }
    return "";
}

char *_inwkt_get_next_closer_comma(stok *token)
{
    char *nextWord = _inwkt_next_word(token);
    if (strcmp(nextWord, ",") == 0 || strcmp(nextWord, ")") == 0) {
        return nextWord;
    }
    return NULL;
}

static void _inwkt_get_precise_coordinates(stok *token, struct Ordinate *flag,
                                           double *coordinates)
{
    coordinates[0] = _inwkt_get_next_number(token);
    coordinates[1] = _inwkt_get_next_number(token);

    // Check for undeclared Z dimension
    if (flag->changeAllowed && (stok_peek_next_token(token) == STOK_NUM))
        ordinate_setZ(flag, true);

    if (flag->value & ORDINATE_VALUE_Z)
        coordinates[2] = _inwkt_get_next_number(token);

    // Check for undeclared M dimension
    if (flag->changeAllowed && (flag->value & ORDINATE_VALUE_Z) &&
        (stok_peek_next_token(token) == STOK_NUM))
        ordinate_setM(flag, true);

    if (flag->value & ORDINATE_VALUE_M)
        coordinates[3] = _inwkt_get_next_number(token);

    flag->changeAllowed = false;
}

struct nv_geobject *_inwkt_read_point(stok *token, struct Ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    _inwkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n == 1) {
        return mg_create_single(0, 1, (flag->value & ORDINATE_VALUE_Z) ? 3 : 2,
                                coord, 0);
    }
    return NULL;
}

struct nv_geobject *_inwkt_read_linestring(stok *token, struct Ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    _inwkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n > 1) {
        return mg_create_single(1, n, (flag->value & ORDINATE_VALUE_Z) ? 3 : 2,
                                coord, 0);
    }
    return NULL;
}

struct nv_geobject *_inwkt_read_linearring(stok *token, struct Ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    _inwkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n > 1) {
        return mg_create_single(2, n, (flag->value & ORDINATE_VALUE_Z) ? 3 : 2,
                                coord, 0);
    }
    return NULL;
}

struct nv_geobject *_inwkt_read_polygon(stok *token, struct Ordinate *flag)
{
    char *nextToken = _inwkt_get_next_empty_or_opener(token, flag);
    if (strncmp(nextToken, "EMPTY", 5) == 0)
        return NULL;

    struct nv_geobject **subs =
        (struct nv_geobject **)calloc(1, sizeof(struct nv_geobject *));
    if (subs == NULL)
        return NULL;
    struct nv_geobject *shell = _inwkt_read_linearring(token, flag);
    subs[0] = shell;
    nextToken = _inwkt_get_next_closer_comma(token);
    while (strcmp(nextToken, ",") == 0) {
    }

    return NULL;
}

struct nv_geobject *_inwkt_read_multipoint(stok *token, struct Ordinate *flag)
{
    return NULL;
}

struct nv_geobject *_inwkt_read_multilinestring(stok *token,
                                                struct Ordinate *flag)
{
    return NULL;
}

struct nv_geobject *_inwkt_read_multipolygon(stok *token, struct Ordinate *flag)
{
    return NULL;
}