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

#ifndef ENCODE_H
#define ENCODE_H

#include <stddef.h>

#ifdef __cpluscplus
extern "C" {
#endif

/// base64
char *base64_encode(const unsigned char *data, size_t len);
unsigned char *base64_decode(const char *text, size_t *out_len);

/// md5
struct MD5_CTX {
    unsigned int lo, hi;
    unsigned int a, b, c, d;
    unsigned char buffer[64];
    unsigned int block[16];
};
void MD5_init(struct MD5_CTX *ctx);
void MD5_update(struct MD5_CTX *ctx, const void *data, unsigned long size);
void MD5_final(unsigned char *result, struct MD5_CTX *ctx);

#ifdef __cpluscplus
}
#endif

#endif