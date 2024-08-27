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

int nv__sda_expand_(char **data, int *length, int *capacity, int memsz)
{
    if (*length + 1 > *capacity) {
        void *ptr;
        int n = (*capacity == 0) ? 1 : *capacity << 1;
        ptr = nv__realloc(*data, n * memsz);
        if (ptr == NULL)
            return -1;
        *data = ptr;
        *capacity = n;
    }
    return 0;
}

int nv__sda_reserve_(char **data, int *length, int *capacity, int memsz, int n)
{
    (void)length;
    if (n > *capacity) {
        void *ptr = nv__realloc(*data, n * memsz);
        if (ptr == NULL)
            return -1;
        *data = ptr;
        *capacity = n;
    }
    return 0;
}

int nv__sda_reserve_po2_(char **data, int *length, int *capacity, int memsz,
                         int n)
{
    int n2 = 1;
    if (n == 0)
        return 0;
    while (n2 < n)
        n2 <<= 1;
    return nv__sda_reserve_(data, length, capacity, memsz, n2);
}

int nv__sda_compact_(char **data, int *length, int *capacity, int memsz)
{
    if (*length == 0) {
        nv__free(*data);
        *data = NULL;
        *capacity = 0;
        return 0;
    }
    else {
        void *ptr;
        int n = *length;
        ptr = nv__realloc(*data, n * memsz);
        if (ptr == NULL)
            return -1;
        *capacity = n;
        *data = ptr;
    }
    return 0;
}

int nv__sda_insert_(char **data, int *length, int *capacity, int memsz, int idx)
{
    int err = nv__sda_expand_(data, length, capacity, memsz);
    if (err)
        return err;
    memmove(*data + (idx + 1) * memsz, *data + idx * memsz,
            (*length - idx) * memsz);
    return 0;
}

void nv__sda_splice_(char **data, int *length, int *capacity, int memsz,
                     int start, int count)
{
    (void)capacity;
    memmove(*data + start * memsz, *data + (start + count) * memsz,
            (*length - start - count) * memsz);
}

void nv__sda_swapsplice_(char **data, int *length, int *capacity, int memsz,
                         int start, int count)
{
    (void)capacity;
    memmove(*data + start * memsz, *data + (*length - count) * memsz,
            count * memsz);
}

void nv__sda_swap_(char **data, int *length, int *capacity, int memsz, int idx1,
                   int idx2)
{
    unsigned char *a, *b, tmp;
    int count;
    (void)length;
    (void)capacity;
    if (idx1 == idx2)
        return;
    a = (unsigned char *)*data + idx1 * memsz;
    b = (unsigned char *)*data + idx2 * memsz;
    count = memsz;
    while (count--) {
        tmp = *a;
        *a = *b;
        *b = tmp;
        a++, b++;
    }
}