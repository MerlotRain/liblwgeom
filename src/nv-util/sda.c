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

#include "sda.h"
#include "nv-common.h"

#define NV__SDA_DEFAULT_CAPACITY (10)

void nv__sda_new(struct nv__sda *a, size_t elem_size,
                 nv__sda_deletor elem_deletor)
{
    a->elem_size = elem_size;
    a->deletor = elem_deletor ? elem_deletor : nv__free;
    a->capacity = NV__SDA_DEFAULT_CAPACITY;
    a->length = 0;
    a->memory =
        (unsigned char *)nv__calloc(NV__SDA_DEFAULT_CAPACITY, elem_size);
}

void nv__sda_free(struct nv__sda *a)
{
}