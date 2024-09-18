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

#ifndef LIBLWGEOM_INTERNEL_H
#define LIBLWGEOM_INTERNEL_H

#include "liblwgeom.h"

#ifndef NV_MAX
#define NV_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef NV_MIN
#define NV_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define NV_MAX3(a, b, c) \
    ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

#define NV_MIN3(a, b, c) \
    ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

// nv-util callback function
typedef void (*DestoryFunc)(void *);
typedef void (*EqualFunc)(const void *, const void *);
typedef void (*HashFunc)(const void *);

// default hash function
size_t lw_str_hash(const void *str);
size_t lw_nearest_pow(size_t v);



#endif /* LIBLWGEOM_INTERNEL_H */