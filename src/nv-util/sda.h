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

#ifndef SDA_H
#define SDA_H

#include <stddef.h>
#include "nv-common.h"
#include <string.h>

#define nv__sda_unpack_(v) \
    (char **)&(v)->data, &(v)->length, &(v)->capacity, sizeof(*(v)->data)

#define nv__sda_(T)           \
    struct {                  \
        T *data;              \
        int length, capacity; \
    }

#define nv__sda_init(v)   memset((v), 0, sizeof(*(v)))

#define nv__sda_deinit(v) (nv__free((v)->data), nv__sda_init(v))

#define nv__sda_push(v, val)                      \
    (nv__sda_expand_(nv__sda_unpack_(v))          \
         ? -1                                     \
         : ((v)->data[(v)->length++] = (val), 0), \
     0)

#define nv__sda_pop(v) (v)->data[--(v)->length]

#define nv__sda_splice(v, start, count) \
    (nv__sda_splice_(nv__sda_unpack_(v), start, count), (v)->length -= (count))

#define nv__sda_swapsplice(v, start, count)                 \
    (nv__sda_swapsplice_(nv__sda_unpack_(v), start, count), \
     (v)->length -= (count))

#define nv__sda_insert(v, idx, val)                                          \
    (nv__sda_insert_(nv__sda_unpack_(v), idx) ? -1                           \
                                              : ((v)->data[idx] = (val), 0), \
     (v)->length++, 0)

#define nv__sda_sort(v, fn) \
    qsort((v)->data, (v)->length, sizeof(*(v)->data), fn)

#define nv__sda_swap(v, idx1, idx2) \
    nv__sda_swap_(nv__sda_unpack_(v), idx1, idx2)

#define nv__sda_truncate(v, len) \
    ((v)->length = (len) < (v)->length ? (len) : (v)->length)

#define nv__sda_clear(v)      ((v)->length = 0)

#define nv__sda_first(v)      (v)->data[0]

#define nv__sda_last(v)       (v)->data[(v)->length - 1]

#define nv__sda_reserve(v, n) nv__sda_reserve_(nv__sda_unpack_(v), n)

#define nv__sda_compact(v)    nv__sda_compact_(nv__sda_unpack_(v))

#define nv__sda_pusharr(v, arr, count)                                        \
    do {                                                                      \
        int i__, n__ = (count);                                               \
        if (nv__sda_reserve_po2_(nv__sda_unpack_(v), (v)->length + n__) != 0) \
            break;                                                            \
        for (i__ = 0; i__ < n__; i__++) {                                     \
            (v)->data[(v)->length++] = (arr)[i__];                            \
        }                                                                     \
    } while (0)

#define nv__sda_extend(v, v2) nv__sda_pusharr((v), (v2)->data, (v2)->length)

#define nv__sda_find(v, val, idx)                       \
    do {                                                \
        for ((idx) = 0; (idx) < (v)->length; (idx)++) { \
            if ((v)->data[(idx)] == (val))              \
                break;                                  \
        }                                               \
        if ((idx) == (v)->length)                       \
            (idx) = -1;                                 \
    } while (0)

#define nv__sda_remove(v, val)           \
    do {                                 \
        int idx__;                       \
        nv__sda_find(v, val, idx__);     \
        if (idx__ != -1)                 \
            nv__sda_splice(v, idx__, 1); \
    } while (0)

#define nv__sda_reverse(v)                                   \
    do {                                                     \
        int i__ = (v)->length / 2;                           \
        while (i__--) {                                      \
            nv__sda_swap((v), i__, (v)->length - (i__ + 1)); \
        }                                                    \
    } while (0)

#define nv__sda_foreach(v, var, iter)                                  \
    if ((v)->length > 0)                                               \
        for ((iter) = 0;                                               \
             (iter) < (v)->length && (((var) = (v)->data[(iter)]), 1); \
             ++(iter))

#define nv__sda_foreach_rev(v, var, iter) \
    if ((v)->length > 0)                  \
        for ((iter) = (v)->length - 1;    \
             (iter) >= 0 && (((var) = (v)->data[(iter)]), 1); --(iter))

#define nv__sda_foreach_ptr(v, var, iter)                               \
    if ((v)->length > 0)                                                \
        for ((iter) = 0;                                                \
             (iter) < (v)->length && (((var) = &(v)->data[(iter)]), 1); \
             ++(iter))

#define nv__sda_foreach_ptr_rev(v, var, iter) \
    if ((v)->length > 0)                      \
        for ((iter) = (v)->length - 1;        \
             (iter) >= 0 && (((var) = &(v)->data[(iter)]), 1); --(iter))

int nv__sda_expand_(char **data, int *length, int *capacity, int memsz);
int nv__sda_reserve_(char **data, int *length, int *capacity, int memsz, int n);
int nv__sda_reserve_po2_(char **data, int *length, int *capacity, int memsz,
                         int n);
int nv__sda_compact_(char **data, int *length, int *capacity, int memsz);
int nv__sda_insert_(char **data, int *length, int *capacity, int memsz,
                    int idx);
void nv__sda_splice_(char **data, int *length, int *capacity, int memsz,
                     int start, int count);
void nv__sda_swapsplice_(char **data, int *length, int *capacity, int memsz,
                         int start, int count);
void nv__sda_swap_(char **data, int *length, int *capacity, int memsz, int idx1,
                   int idx2);

typedef nv__sda_(void *) nv__sda_void;
typedef nv__sda_(char *) nv__sda_str;
typedef nv__sda_(int) nv__sda_int;
typedef nv__sda_(char) nv__sda_char;
typedef nv__sda_(float) nv__sda_float;
typedef nv__sda_(double) nv__sda_double;

#endif