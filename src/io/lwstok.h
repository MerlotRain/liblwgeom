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

#ifndef STOK_H
#define STOK_H

#include <stddef.h>

enum {
    UV__STOK_EOF,  // end of string stream
    UV__STOK_EOL,  // end of line
    UV__STOK_NUM,  // token a number value
    UV__STOK_WORD, // token a string value
};

struct uv__stok {
    double ntok;
    char stok[255];
    size_t len;
    char *head; // string stream head pointer
    char *pos;  // string stream current pointer
    char *end;  // string stream end pointer
};

void uv__stok_init(struct uv__stok *tok, char *t);
int uv__stok_next_token(struct uv__stok *tok);
int uv__stok_peek_next_token(struct uv__stok *tok);

#endif