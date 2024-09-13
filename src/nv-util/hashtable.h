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

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include "nv-common.h"

struct nv__hashtable *nv__hashtable_new(HashFunc hash_func, EqualFunc key_equal_func, 
        DestoryFunc key_destroy_func, DestoryFunc value_destroy_func);

void nv__hashtable_free(struct nv__hashtable *hash_table);
NV_BOOLEAN nv__hashtable_insert(struct nv__hashtable *hash_table, void *key, void *value);
NV_BOOLEAN nv__hashtable_replace(struct nv__hashtable *hash_table, void* key, void *value);
NV_BOOLEAN nv__hashtable_remove(struct nv__hashtable *hash_table, void* key);
NV_BOOLEAN nv__hashtable_remove_all(struct nv__hashtable *hash_table);
NV_BOOLEAN nv__hashtable_contains(struct nv__hashtable *hash_table, const void *key);

#endif