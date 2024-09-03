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

#include "geom-internal.h"

#include "ordinate.h"
#include "stok.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

/* ----------------------------- static read wkt ---------------------------- */

static char *nv__wkt_next_word(struct uv__stok *token);

static double nv__wkt_get_next_number(struct uv__stok *token);

static void nv__wkt_get_coordinates(struct uv__stok *token,
                                    struct uv__ordinate *flag,
                                    double **coordinates, int *num);

static void nv__wkt_get_precise_coordinates(struct uv__stok *token,
                                            struct uv__ordinate *flag,
                                            double *coordinates);

static char *nv__wkt_get_next_empty_or_opener(struct uv__stok *token,
                                              struct uv__ordinate *flag);

static char *nv__wkt_get_next_closer_comma(struct uv__stok *token);

static struct nv_geom *nv__wkt_read_point(struct uv__stok *token,
                                          struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_linestring(struct uv__stok *token,
                                               struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_linearring(struct uv__stok *token,
                                               struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_polygon(struct uv__stok *token,
                                            struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_multipoint(struct uv__stok *token,
                                               struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_multilinestring(struct uv__stok *token,
                                                    struct uv__ordinate *flag);

static struct nv_geom *nv__wkt_read_multipolygon(struct uv__stok *token,
                                                 struct uv__ordinate *flag);

/* -------------------------------- inner wkt ------------------------------- */

struct nv_geom *nv__geo_read_wkt(const char *data, size_t len)
{
    char *p = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "C");

    struct uv__stok token;
    uv__stok_init(&token, (char *)data);

    struct uv__ordinate flags = uv__ordinate_XY();
    struct uv__ordinate new_flags = uv__ordinate_XY();
    char *type = nv__wkt_next_word(&token);
    if (strcmp(type, "EMPTY") == 0) {
        memcpy(&new_flags, &flags, sizeof(struct uv__ordinate));
    }
    else {
        if (strlen(type) >= 2 &&
            0 == strncmp(type + strlen(type) - 2, "ZM", 2)) {
            uv__ordinate_setZ(&new_flags, NV_TRUE);
            uv__ordinate_setM(&new_flags, NV_TRUE);
            new_flags.changeAllowed = NV_FALSE;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "M", 1)) {
            uv__ordinate_setM(&new_flags, NV_TRUE);
            new_flags.changeAllowed = NV_FALSE;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "Z", 1)) {
            uv__ordinate_setZ(&new_flags, NV_TRUE);
            new_flags.changeAllowed = NV_FALSE;
        }
    }

    if (strncmp(type, "POINT", 5) == 0) {
        return nv__wkt_read_point(&token, &new_flags);
    }
    else if (strncmp(type, "LINESTRING", 10) == 0) {
        return nv__wkt_read_linestring(&token, &new_flags);
    }
    else if (strncmp(type, "LINEARRING ", 10) == 0) {
        return nv__wkt_read_linearring(&token, &new_flags);
    }
    else if (strncmp(type, "POLYGON", 7) == 0) {
        return nv__wkt_read_polygon(&token, &new_flags);
    }
    else if (strncmp(type, "MULTIPOINT", 10) == 0) {
        return nv__wkt_read_multipoint(&token, &new_flags);
    }
    else if (strncmp(type, "MULTILINESTRING", 15) == 0) {
        return nv__wkt_read_multilinestring(&token, &new_flags);
    }
    else if (strncmp(type, "MULTIPOLYGON", 12) == 0) {
        return nv__wkt_read_multipolygon(&token, &new_flags);
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

char *nv__wkt_next_word(struct uv__stok *token)
{
    int type = uv__stok_next_token(token);
    switch (type) {
    case UV__STOK_EOF:
    case UV__STOK_EOL:
    case UV__STOK_NUM:
        return "";
    case UV__STOK_WORD: {
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

double nv__wkt_get_next_number(struct uv__stok *token)
{
    int type = uv__stok_next_token(token);
    switch (type) {
    case UV__STOK_NUM:
        return token->ntok;
    default:
        return 0.0;
    }
    return 0;
}

void nv__wkt_get_coordinates(struct uv__stok *token, struct uv__ordinate *flag,
                             double **coordinates, int *num)
{
    char *nexttok = nv__wkt_get_next_empty_or_opener(token, flag);
    if (strcmp(nexttok, "EMPTY") == 0) {
        return;
    }
    double c[4] = {0.0};
    nv__wkt_get_precise_coordinates(token, flag, c);

    // calloc coordinates with cdim
    int cdim = (flag->value & UV__ORDINATE_VALUE_Z) ? 3 : 2;
    *coordinates = (double *)nv__calloc(cdim, sizeof(double));
    if (*coordinates == NULL) {
        return;
    }
    *num = 1;
    memcpy(*coordinates, c, cdim * sizeof(double));

    nexttok = nv__wkt_get_next_closer_comma(token);
    while (strcmp(nexttok, ")") != 0) {
        memset(c, 0, sizeof(c));
        nv__wkt_get_precise_coordinates(token, flag, c);
        *coordinates = (double *)nv__realloc(
            *coordinates, (*num + 1) * sizeof(double) * (cdim));
        if (*coordinates == NULL) {
            nv__free(*coordinates);
            *num = 0;
            // log error
            return;
        }
        memcpy((*coordinates) + ((*num) * cdim), c, cdim * sizeof(double));
        (*num)++;
        nexttok = nv__wkt_get_next_closer_comma(token);
    }
}

char *nv__wkt_get_next_empty_or_opener(struct uv__stok *token,
                                       struct uv__ordinate *flag)
{
    char *nextword = nv__wkt_next_word(token);
    if (strcmp(nextword, "ZM") == 0) {
        nextword = nv__wkt_next_word(token);
    }
    else {
        if (strcmp(nextword, "Z") == 0) {
            nextword = nv__wkt_next_word(token);
        }
        if (strcmp(nextword, "M") == 0) {
            nextword = nv__wkt_next_word(token);
        }
    }

    if (strcmp(nextword, "(") == 0 || strcmp(nextword, "EMPTY") == 0) {
        return nextword;
    }
    return "";
}

char *nv__wkt_get_next_closer_comma(struct uv__stok *token)
{
    char *nextWord = nv__wkt_next_word(token);
    if (strcmp(nextWord, ",") == 0 || strcmp(nextWord, ")") == 0) {
        return nextWord;
    }
    return NULL;
}

static void nv__wkt_get_precise_coordinates(struct uv__stok *token,
                                            struct uv__ordinate *flag,
                                            double *coordinates)
{
    coordinates[0] = nv__wkt_get_next_number(token);
    coordinates[1] = nv__wkt_get_next_number(token);

    // Check for undeclared Z dimension
    if (flag->changeAllowed &&
        (uv__stok_peek_next_token(token) == UV__STOK_NUM))
        uv__ordinate_setZ(flag, NV_TRUE);

    if (flag->value & UV__ORDINATE_VALUE_Z)
        coordinates[2] = nv__wkt_get_next_number(token);

    // Check for undeclared M dimension
    if (flag->changeAllowed && (flag->value & UV__ORDINATE_VALUE_Z) &&
        (uv__stok_peek_next_token(token) == UV__STOK_NUM))
        uv__ordinate_setM(flag, NV_TRUE);

    if (flag->value & UV__ORDINATE_VALUE_M)
        coordinates[3] = nv__wkt_get_next_number(token);

    flag->changeAllowed = NV_FALSE;
}

struct nv_geom *nv__wkt_read_point(struct uv__stok *token,
                                   struct uv__ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    nv__wkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n == 1) {
        // return nv_geo_create_single(
        //     0, 1, (flag->value & UV__ORDINATE_VALUE_Z) ? 3 : 2, coord, 0);
    }
    return NULL;
}

struct nv_geom *nv__wkt_read_linestring(struct uv__stok *token,
                                        struct uv__ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    nv__wkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n > 1) {
        // return nv_geo_create_single(
        //     1, n, (flag->value & UV__ORDINATE_VALUE_Z) ? 3 : 2, coord, 0);
    }
    return NULL;
}

struct nv_geom *nv__wkt_read_linearring(struct uv__stok *token,
                                        struct uv__ordinate *flag)
{
    double *coord = NULL;
    int n = 0;
    nv__wkt_get_coordinates(token, flag, &coord, &n);
    if (coord && n > 1) {
        // return nv_geo_create_single(
        //     2, n, (flag->value & UV__ORDINATE_VALUE_Z) ? 3 : 2, coord, 0);
    }
    return NULL;
}

struct nv_geom *nv__wkt_read_polygon(struct uv__stok *token,
                                     struct uv__ordinate *flag)
{
    char *nextToken = nv__wkt_get_next_empty_or_opener(token, flag);
    if (strncmp(nextToken, "EMPTY", 5) == 0)
        return NULL;

    struct nv_geom **subs =
        (struct nv_geom **)nv__calloc(1, sizeof(struct nv_geom *));
    if (subs == NULL)
        return NULL;
    struct nv_geom *shell = nv__wkt_read_linearring(token, flag);
    subs[0] = shell;
    nextToken = nv__wkt_get_next_closer_comma(token);
    while (strcmp(nextToken, ",") == 0) {
    }

    return NULL;
}

struct nv_geom *nv__wkt_read_multipoint(struct uv__stok *token,
                                        struct uv__ordinate *flag)
{
    return NULL;
}

struct nv_geom *nv__wkt_read_multilinestring(struct uv__stok *token,
                                             struct uv__ordinate *flag)
{
    return NULL;
}

struct nv_geom *nv__wkt_read_multipolygon(struct uv__stok *token,
                                          struct uv__ordinate *flag)
{
    return NULL;
}