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

#ifndef LIBNV_COMMON_H
#define LIBNV_COMMON_H

#include <stdio.h>
#include <math.h>
#include <nv.h>
#include "logging.h"

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

#define NV_DBL_NEAR(a, b) (fabs((a) - (b)) < 4 * DBL_EPSILON)

#ifndef NV_FALLTHROUGH
#if (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 700) && \
    !defined(__INTEL_COMPILER)
#define NV_FALLTHROUGH __attribute__((fallthrough));
#else
#define NV_FALLTHROUGH ;
#endif
#endif

/* Allocator prototypes */
void *nv__calloc(size_t count, size_t size);
void *nv__malloc0(size_t size);
void *nv__malloc(size_t size);
void nv__free(void *ptr);
void *nv__realloc(void *ptr, size_t size);
void nv__record_error(int code, const char *fmt, ...);

size_t nv__nearest_pow(size_t v);

// nv-util callback function
typedef void (*DestoryFunc)(void *);
typedef void (*EqualFunc)(const void *, const void *);
typedef void (*HashFunc)(const void *);

// default hash function
size_t nv__str_hash(const void *str);

#endif /* LIBNV_COMMON_H */
