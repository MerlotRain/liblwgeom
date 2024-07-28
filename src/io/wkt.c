#include "geom.h"

#include <string.h>
#include <locale.h>

#include "stok.h"
#include <assert.h>
#include <ctype.h>

#define str_endswith(str, suffix)

struct Ordinate {
    int value;
    bool changeAllowed;
};

static char *wkt_next_word(stok *token);

static void wkt_get_coordinates(stok *token, double **coordinates, int *num);

static char *wkt_get_next_empty_or_opener(stok *token);

se_geom *geom_read_wkt(const char *data, int len)
{
#ifdef _MSC_VER
    // Avoid multithreading issues caused by setlocale
    _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif
    char *p = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "C");

    stok token;
    stok_init(&token, (char *)data);

    struct Ordinate flags;
    struct Ordinate new_flags;
    flags.value = 1 | 2;
    flags.changeAllowed = true;

    char *type = wkt_next_word(&token);
    if (strcmp(type, "EMPTY") == 0) {
        memcpy(&new_flags, &flags, sizeof(struct Ordinate));
    }
    else {
        if (strlen(type) >= 2 &&
            0 == strncmp(type + strlen(type) - 2, "ZM", 2)) {
            new_flags.value ^= 4;
            new_flags.value ^= 8;
            new_flags.changeAllowed = false;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "M", 1)) {
            new_flags.value ^= 4;
            new_flags.changeAllowed = false;
        }
        else if (strlen(type) >= 1 &&
                 0 == strncmp(type + strlen(type) - 1, "Z", 1)) {
            new_flags.value ^= 8;
            new_flags.changeAllowed = false;
        }
    }

    if (strncmp(type, "POINT", 5) == 0) {
    }
    else if (strncmp(type, "LINESTRING", 10) == 0) {
    }
    else if (strncmp(type, "LINEARRING", 10) == 0) {
    }
    else if (strncmp(type, "CIRCULARSTRING", 14) == 0) {
    }
    else if (strncmp(type, "COMPOUNDCURVE", 13) == 0) {
    }
    else if (strncmp(type, "POLYGON", 7) == 0) {
    }
    else if (strncmp(type, "CURVEPOLYGON", 12) == 0) {
    }
    else if (strncmp(type, "MULTIPOINT", 10) == 0) {
    }
    else if (strncmp(type, "MULTILINESTRING", 15) == 0) {
    }
    else if (strncmp(type, "MULTICURVE", 10) == 0) {
    }
    else if (strncmp(type, "MULTIPOLYGON", 12) == 0) {
    }
    else if (strncmp(type, "MULTISURFACE", 12) == 0) {
    }
    else if (strncmp(type, "GEOMETRYCOLLECTION", 18) == 0) {
    }

    setlocale(LC_NUMERIC, p);
    return NULL;
}

static char *wkt_next_word(stok *token)
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

static void wkt_get_coordinates(stok *token, double **coordinates, int *num)
{
    char *nexttok = wkt_get_next_empty_or_opener(token);
}

char *wkt_get_next_empty_or_opener(stok *token)
{
    char *nextword = wkt_next_word(token);
    if (strcmp(nextword, "ZM") == 0) {
        nextword = wkt_next_word(token);
    }
    else {
        if (strcmp(nextword, "Z") == 0) {
            nextword = wkt_next_word(token);
        }
        if (strcmp(nextword, "M") == 0) {
            nextword = wkt_next_word(token);
        }
    }

    if (strcmp(nextword, "(") == 0 || strcmp(nextword, "EMPTY") == 0) {
        return nextword;
    }
    return "";
}
