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

#include "nv-common.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    nv_malloc_func local_malloc;
    nv_realloc_func local_realloc;
    nv_calloc_func local_calloc;
    nv_free_func local_free;
} nv__allocator_t;

static nv__allocator_t nv__allocator = {
    malloc,
    realloc,
    calloc,
    free,
};

int uv_replace_allocator(nv_malloc_func malloc_func,
                         nv_realloc_func realloc_func,
                         nv_calloc_func calloc_func, nv_free_func free_func)
{
    if (malloc_func == NULL || realloc_func == NULL || calloc_func == NULL ||
        free_func == NULL) {
        return -1;
    }

    nv__allocator.local_malloc = malloc_func;
    nv__allocator.local_realloc = realloc_func;
    nv__allocator.local_calloc = calloc_func;
    nv__allocator.local_free = free_func;

    return 0;
}

void *nv__malloc(size_t size)
{
    if (size > 0)
        return nv__allocator.local_malloc(size);
    return NULL;
}

void nv__free(void *ptr)
{
    nv__allocator.local_free(ptr);
}

void *nv__calloc(size_t count, size_t size)
{
    return nv__allocator.local_calloc(count, size);
}

void *nv__realloc(void *ptr, size_t size)
{
    if (size > 0)
        return nv__allocator.local_realloc(ptr, size);
    nv__free(ptr);
    return NULL;
}

static char last_error[2048];

static int last_errno = 0;

void nv_record_error(int code, const char *fmt, ...)
{
    memset(last_error, 0, 2048);
    va_list args;
    va_start(args, fmt);
    vsnprintf(last_error, sizeof(last_error), fmt, args);
    va_end(args);
}

int error_code()
{
    return last_errno;
}

char *error_messgae()
{
    return last_error;
}