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

/* https://github.com/rxi/vec.git
   A type-safe dynamic array implementation for C.
*/

#include "sda.h"
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

struct nv__sda_real
{
    char* data;
    size_t len;
    size_t elt_capacity;
    size_t elt_size;
    DestoryFunc clear_func;
};

#define nv__sda_elt_len(array, i) ((size_t)(array)->elt_size * (i))
#define nv__sda_elt_pos(array, i) ((array)->data + nv__sda_elt_len((array), (i)))
#define nv__sda_elt_zero(array, pos, len) \
    (memset(nv__sda_elt_pos((array), pos), 0, nv__sda_elt_len((array), len)))

static void nv__sda_maybe_expand(struct nv__sda_real *array, size_t len);

struct nv__sda *nv__sda_new(size_t element_size)
{
    if(element_size <= 0 || element_size >= 128)
        return NULL;
    
    struct nv__sda_real* array = (struct nv__sda_real*)nv__malloc(sizeof(struct nv__sda_real));
    if(!array)
        return NULL;

    array->data = NULL;
    array->len = 0;
    array->elt_capacity = 0;
    array->elt_size = element_size;
    array->clear_func = 0;
    return (struct nv__sda *)array;
}

char *nv__sda_free(struct nv__sda *a, NV_BOOLEAN free_segment)
{
    struct nv__sda_real *array = (struct nv__sda_real *) a;
    if (!array) return NULL;

    char *segment;
    if (free_segment)
    {
        if (array->clear_func != NULL)
        {
            for (size_t i = 0; i < array->len; i++)
                array->clear_func(nv__sda_elt_pos(array, i));
        }

        nv__free(array->data);
        segment = NULL;
    }
    else
    {
        segment = (char *) array->data;
    }
    
    nv__free(array);
    return segment;
}

struct nv__sda *nv__sda_append_vals(struct nv__sda *a, void *data, size_t len)
{
    struct nv__sda_real *array = (struct nv__sda_real*)a;
    if(!array)
        return NULL;

    if(len == 0)
        return a;

    nv__sda_maybe_expand(array, len);
    memcpy(nv__sda_elt_pos(array, array->len), data, nv__sda_elt_len(array, len));

    array->len += len;
    return a;
}

struct nv__sda *nv__sda_prepend_vals(struct nv__sda *a, void *data, size_t len)
{
    struct nv__sda_real *array = (struct nv__sda_real *)a;
    if(array == NULL)
        return NULL;

    if (len == 0)
        return a;
    
    nv__sda_maybe_expand(array, len);

    memmove(nv__sda_elt_pos(array, len), nv__sda_elt_pos(array, 0),
            nv__sda_elt_len(array, array->len));

    memcpy(nv__sda_elt_pos(array, 0), data, nv__sda_elt_len(array, len));

    array->len += len;

    return a;
}

struct nv__sda *nv__sda_insert_vals(struct nv__sda *a, size_t index_,
                                    void *data, size_t len)
{
    struct nv__sda_real *array = (struct nv__sda_real *) a;
    if (!array) return NULL;

    if (len == 0) return a;

    /* Is the index off the end of the array, and hence do we need to over-allocate
   * and clear some elements? */
    if (index_ >= array->len)
    {
        nv__sda_maybe_expand(array, index_ - array->len + len);
        return nv__sda_append_vals(nv__sda_set_size(a, index_), data, len);
    }

    nv__sda_maybe_expand(array, len);

    memmove(nv__sda_elt_pos(array, len + index_),
            nv__sda_elt_pos(array, index_),
            nv__sda_elt_len(array, array->len - index_));

    memcpy(nv__sda_elt_pos(array, index_), data, nv__sda_elt_len(array, len));

    array->len += len;

    return a;            
}

struct nv__sda *nv__sda_set_size(struct nv__sda *a, size_t length)
{
    struct nv__sda_real *array = (struct nv__sda_real *) a;
    if (!array) return NULL;

    if (length > array->len)
    {
        nv__sda_maybe_expand(array, length - array->len);
    }
    else if (length < array->len)
        nv__sda_remove_range(a, length, array->len - length);

    array->len = length;

    return a;
}

struct nv__sda *nv__sda_remove_range(struct nv__sda *a, size_t index_,
                                     size_t length)
{
    struct nv__sda_real *array = (struct nv__sda_real *) a;
    if (!array) return NULL;
    if (index_ > array->len) return NULL;
    if (index_ > UINT_MAX - length) return NULL;
    if (index_ + length > array->len) return NULL;

    if (array->clear_func != NULL)
    {
        size_t i;
        for (i = 0; i < length; i++)
            array->clear_func(nv__sda_elt_pos(array, index_ + i));
    }

    if (index_ + length != array->len)
        memmove(nv__sda_elt_pos(array, index_),
                nv__sda_elt_pos(array, index_ + length),
                (array->len - (index_ + length)) * array->elt_size);

    array->len -= length;
    nv__sda_elt_zero(array, array->len, length);

    return a;  
}

void nv__sda_set_clear_func(struct nv__sda *a, , DestoryFunc func)
{
    struct nv__sda_real *array = (struct nv__sda_real*)(a);
    if(array == NULL)
        return;

    array->clear_func = func;
}

static void nv__sda_maybe_expand(struct nv__sda_real *array, size_t len)
{
    size_t max_len, want_len;
    max_len = NV_MIN(SIZE_MAX / 2 / array->elt_size, UINT_MAX);

        /* Detect potential overflow */
    if ((max_len - array->len) < len) abort();

    want_len = array->len + len;
    if (want_len > array->elt_capacity)
    {
        size_t want_alloc = nv__nearest_pow(nv__sda_elt_len(array, want_len));
        assert(want_alloc >= nv__sda_elt_len(array, want_len));
        want_alloc  = NV_MAX(want_alloc, 16);

        array->data = nv__realloc(array->data, want_alloc);

        memset(nv__sda_elt_pos(array, array->elt_capacity), 0,
               nv__sda_elt_len(array, want_len - array->elt_capacity));

        array->elt_capacity = NV_MIN(want_alloc / array->elt_size, UINT_MAX);
    }
}