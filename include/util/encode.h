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

#ifndef ENCODE_H
#define ENCODE_H

#ifdef __cpluscplus
extern "C" {
#endif

#include <mathse.h>

/// base64

EXTERN char *base64_encode(const unsigned char *data, size_t len);
EXTERN unsigned char *base64_decode(const char *text, size_t *out_len);

/// md5

struct MD5_CTX {
    unsigned int lo, hi;
    unsigned int a, b, c, d;
    unsigned char buffer[64];
    unsigned int block[16];
};
EXTERN void MD5_init(struct MD5_CTX *ctx);
EXTERN void MD5_update(struct MD5_CTX *ctx, const void *data,
                       unsigned long size);
EXTERN void MD5_final(unsigned char *result, struct MD5_CTX *ctx);

#ifdef __cpluscplus
}
#endif

#endif