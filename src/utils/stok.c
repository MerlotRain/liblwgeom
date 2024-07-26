/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
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

#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <limits.h>

static double strtod_with_vc_fix(const char* str, char** str_end)
{
    double dbl = strtod(str, str_end);
#if _MSC_VER && !__INTEL_COMPILER
    if(*str_end[0] != '\0') {
        char sign = 0;
        const char* pos = str;
        if(*pos == '+' || *pos == '-') {
            sign = *pos++;
        }

        if(stricmp(pos, "inf") == 0) {
            if(!sign || sign == '+') {
                dbl = DoubleInfinity;
            }
            else {
                dbl = DoubleNegInfinity;
            }
            *str_end[0] = '\0';
        }
        else if(stricmp(pos, "nan") == 0) {
            dbl = DoubleNotANumber;
            *str_end[0] = '\0';
        }
    }
#endif
    return dbl;
}

void stok_init(stok *tok, char *t) {
    tok->buf = t;
    tok->len = strlen(t);
    tok->offset = 0;
    tok->end = tok->buf + tok->len;
    memset(tok->stok, 0, UCHAR_MAX);
    tok->ntok = 0.;
}


int next_token(stok *tok) {
    if (tok->buf == '\0')
        return 0;

    switch (*tok->buf) {
    case '(':
    case ')':
    case ',':
        (tok->buf)++;
        (tok->offset)++;
        return tok->offset;
    case '\n':
    case '\r':
    case '\t':
    case ' ':
        int pos = strspn(tok->buf, " \n\r\t");
        if (pos == 0)
            return 0;
        else {
            tok->buf += pos;
            return next_token(tok);
        }
    }

    char *brk = strpbrk(tok->buf, "\n\r\t() ,");
    if (brk == tok->buf) {
        if (tok->buf != tok->end) {
            if (tok->len - tok->offset >= UCHAR_MAX)
                return -1;
            memcpy(tok->stok, tok->buf, strlen(tok->buf));
            tok->buf = tok->end;
        } else {
            return 0;
        }
    } else {
        memcpy(tok->stok, tok->buf, brk - tok->buf);
        tok->buf = brk;
    }

    char  *stopstring;
    double dbl = strtod_with_vc_fix(tok->stok, &stopstring);
    if (*stopstring == '\0') {
        tok->ntok = dbl;
        memset(tok->stok, 0, UCHAR_MAX);
        return 2;
    } else {
        return 3;
    }
}
