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

#include "sort.h"

#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

#include <stdlib.h>

struct qsort_param {
    size_t s;
    size_t var;
    int (*compare)(const void *, const void *);
    void *args;
    char *t;
};

static void quick_sort_r(void *b, size_t n, size_t s,
                         int (*compare)(const void *, const void *), void *arg)
{

    size_t size = n * s;
    char *tmp = NULL;
    struct qsort_param p;

    if (s > 32)
        size = 2 * n * sizeof(ptrdiff_t *) + s;

    if (size < 1024) {
        p.t = (char *)alloca(size);
    }
    else {
        tmp = (char *)malloc(size);
        if (tmp != NULL) {
            p.t = tmp;
        }
    }
}

void quick_sort(const void *p, size_t ele_count, size_t ele_size,
                int (*compare)(const void *, const void *), void *user_data)
{
    quick_sort_r((void *)p, ele_count, ele_size, compare, user_data);
}
