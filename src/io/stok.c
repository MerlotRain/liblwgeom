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

#include "stok.h"
#include <limits.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static double stok__strtod_with_vc_fix(const char *str, char **str_end)
{
    double dbl = strtod(str, str_end);
#if _MSC_VER && !__INTEL_COMPILER
    if (*str_end[0] != '\0') {
        char sign = 0;
        const char *pos = str;
        if (*pos == '+' || *pos == '-') {
            sign = *pos++;
        }

        if (stricmp(pos, "inf") == 0) {
            if (!sign || sign == '+') {
                dbl = INFINITY;
            }
            else {
                dbl = INFINITY;
            }
            *str_end[0] = '\0';
        }
        else if (stricmp(pos, "nan") == 0) {
            dbl = NAN;
            *str_end[0] = '\0';
        }
    }
#endif
    return dbl;
}

void stok_init(stok_t *tok, char *t)
{
    tok->head = t;                   // head of the string
    tok->pos = t;                    // current position
    tok->len = strlen(t);            // length of the string
    tok->end = tok->pos + tok->len;  // end of the string
    memset(tok->stok, 0, UCHAR_MAX); // current token
    tok->ntok = 0.;                  // current token as a number
}

int stok_next_token(stok_t *tok)
{
    if (tok->pos == tok->end)
        return STOK_EOF;

    switch (*tok->pos) {
    case '(':
    case ')':
    case ',':
        return *tok->pos++;
    case '\n':
    case '\r':
    case '\t':
    case ' ': {
        size_t _pos = strspn(tok->pos, " \n\r\t");
        if (_pos == strlen(tok->pos))
            return STOK_EOF;
        else {
            tok->pos = tok->pos + _pos;
            return stok_next_token(tok);
        }
    }
    }

    char *brk = strpbrk(tok->pos, "\n\r\t() ,");
    if (brk == tok->end) {
        if (tok->pos != tok->end) {
            memcpy(tok->stok, tok->pos, strlen(tok->pos));
            tok->pos = tok->end;
        }
        else {
            return STOK_EOF;
        }
    }
    else {
        memset(tok->stok, 0, strlen(tok->stok));
        memcpy(tok->stok, tok->pos, brk - tok->pos);
        tok->pos = brk;
    }

    char *stopstring;
    double dbl = stok__strtod_with_vc_fix(tok->stok, &stopstring);
    if (*stopstring == '\0') {
        tok->ntok = dbl;
        memset(tok->stok, 0, UCHAR_MAX);
        return STOK_NUM;
    }
    else {
        return STOK_WORD;
    }
}

int stok_peek_next_token(stok_t *tok)
{
    if (tok->pos == tok->end)
        return STOK_EOF;

    char stok[UCHAR_MAX] = {0};
    size_t _pos = strspn(tok->pos, " \n\r\t");
    if (_pos == strlen(tok->pos))
        return STOK_EOF;

    switch (*tok->pos)
    case '(':
    case ')':
    case ',':
        return *tok->pos;

    char *brk = strpbrk(tok->pos + 1, "\n\r\t() ,");
    if (brk == tok->end) {
        if (tok->pos != tok->end)
            memcpy(stok, tok->pos, strlen(tok->pos));
        else
            return STOK_EOF;
    }
    else {
        memcpy(stok, tok->pos, brk - tok->pos);
    }

    char *stopstring;
    double dbl = stok__strtod_with_vc_fix(stok, &stopstring);
    if (*stopstring == '\0') {
        tok->ntok = dbl;
        memset(tok->stok, 0, strlen(tok->stok));
        return STOK_NUM;
    }
    else {
        tok->ntok = 0.0;
        memcpy(tok->stok, stok, strlen(stok));
        return STOK_WORD;
    }
}
