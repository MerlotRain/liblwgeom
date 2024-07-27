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

#ifndef STOK_H
#define STOK_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "mathse.h"

#define STOK_EOF  0 // end of string stream
#define STOK_EOL  1 // end of line
#define STOK_NUM  2 // token a number value
#define STOK_WORD 3 // token a string value

typedef struct _intstok {
    double ntok;
    char stok[255];
    int len;
    char *head; // string stream head pointer
    char *pos;  // string stream current pointer
    char *end;  // string stream end pointer
} stok;

void stok_init(stok *tok, char *t);
int stok_next_token(stok *tok);
int stok_peek_next_token(stok *tok);

#ifdef __cpluscplus
}
#endif

#endif