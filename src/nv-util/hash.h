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

#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct nv__hashmap;

struct nv__hashmap *
nv__hashmap_new(size_t elsize, size_t cap, uint64_t seed0, uint64_t seed1,
            uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1),
            int (*compare)(const void *a, const void *b, void *udata),
            void (*elfree)(void *item), void *udata);

void nv__hashmap_free(struct nv__hashmap *map);
void nv__hashmap_clear(struct nv__hashmap *map, bool update_cap);
size_t nv__hashmap_count(struct nv__hashmap *map);
bool nv__hashmap_oom(struct nv__hashmap *map);
const void *nv__hashmap_get(struct nv__hashmap *map, const void *item);
const void *nv__hashmap_set(struct nv__hashmap *map, const void *item);
const void *nv__hashmap_delete(struct nv__hashmap *map, const void *item);
const void *nv__hashmap_probe(struct nv__hashmap *map, uint64_t position);
bool nv__hashmap_scan(struct nv__hashmap *map,
                  bool (*iter)(const void *item, void *udata), void *udata);
bool nv__hashmap_iter(struct nv__hashmap *map, size_t *i, void **item);

uint64_t nv__hashmap_sip(const void *data, size_t len, uint64_t seed0,
                     uint64_t seed1);
uint64_t nv__hashmap_murmur(const void *data, size_t len, uint64_t seed0,
                        uint64_t seed1);
uint64_t nv__hashmap_xxhash3(const void *data, size_t len, uint64_t seed0,
                         uint64_t seed1);

const void *nv__hashmap_get_with_hash(struct nv__hashmap *map, const void *key,
                                  uint64_t hash);
const void *nv__hashmap_delete_with_hash(struct nv__hashmap *map, const void *key,
                                     uint64_t hash);
const void *nv__hashmap_set_with_hash(struct nv__hashmap *map, const void *item,
                                  uint64_t hash);
void nv__hashmap_set_grow_by_power(struct nv__hashmap *map, size_t power);
void nv__hashmap_set_load_factor(struct nv__hashmap *map, double load_factor);

#endif
