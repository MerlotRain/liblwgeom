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

#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>

enum {
    NV__BITSET_STATE_NONE, ///< All bits are NV_FALSE
    NV__BITSET_STATE_ALL,  ///< All bits are NV_TRUE
    NV__BITSET_STATE_ANY   ///< Some bits are NV_TRUE
};

struct nv__bitset;

struct nv__bitset *nv__bitset_new(size_t size);
void nv__bitset_free(struct nv__bitset *bs);
void nv__bitset_set(struct nv__bitset *bs, size_t index);
void nv__bitset_clear(struct nv__bitset *bs, size_t index);
int nv__bitset_test(struct nv__bitset *bs, size_t index);
void nv__bitset_flip(struct nv__bitset *bs, size_t index);
int nv__bitset_state(struct nv__bitset *bs);
size_t nv__bitset_count(struct nv__bitset *bs);
size_t nv__bitset_size(struct nv__bitset *bs);

#endif