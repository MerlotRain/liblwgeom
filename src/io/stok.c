/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "stok.h"
#include <limits.h>

static double strtod_with_vc_fix(const char *str, char **str_end)
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

void stok_init(stok *tok, char *t)
{
    tok->head = t;                   // head of the string
    tok->pos = t;                    // current position
    tok->len = strlen(t);            // length of the string
    tok->end = tok->pos + tok->len;  // end of the string
    memset(tok->stok, 0, UCHAR_MAX); // current token
    tok->ntok = 0.;                  // current token as a number
}

int stok_next_token(stok *tok)
{
    if (tok->pos == tok->end)
        return STOK_EOF;

    switch (*tok->pos) {
    case '(':
    case ')':
    case ',':
        return *tok->pos;
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
    double dbl = strtod_with_vc_fix(tok->stok, &stopstring);
    if (*stopstring == '\0') {
        tok->ntok = dbl;
        memset(tok->stok, 0, UCHAR_MAX);
        return STOK_NUM;
    }
    else {
        return STOK_WORD;
    }
}

int stok_peek_next_token(stok *tok)
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
    double dbl = strtod_with_vc_fix(stok, &stopstring);
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
