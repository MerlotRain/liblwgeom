/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef HASH_H
#define HASH_H

#ifdef __cpluscplus
extern "C" {
#endif

#include <mathse.h>

struct hashmap;

struct hashmap *
hashmap_new(size_t elsize, size_t cap, uint64_t seed0, uint64_t seed1,
            uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1),
            int (*compare)(const void *a, const void *b, void *udata),
            void (*elfree)(void *item), void *udata);

void hashmap_free(struct hashmap *map);
void hashmap_clear(struct hashmap *map, bool update_cap);
size_t hashmap_count(struct hashmap *map);
bool hashmap_oom(struct hashmap *map);
const void *hashmap_get(struct hashmap *map, const void *item);
const void *hashmap_set(struct hashmap *map, const void *item);
const void *hashmap_delete(struct hashmap *map, const void *item);
const void *hashmap_probe(struct hashmap *map, uint64_t position);
bool hashmap_scan(struct hashmap *map,
                  bool (*iter)(const void *item, void *udata), void *udata);
bool hashmap_iter(struct hashmap *map, size_t *i, void **item);

uint64_t hashmap_sip(const void *data, size_t len, uint64_t seed0,
                     uint64_t seed1);
uint64_t hashmap_murmur(const void *data, size_t len, uint64_t seed0,
                        uint64_t seed1);
uint64_t hashmap_xxhash3(const void *data, size_t len, uint64_t seed0,
                         uint64_t seed1);

const void *hashmap_get_with_hash(struct hashmap *map, const void *key,
                                  uint64_t hash);
const void *hashmap_delete_with_hash(struct hashmap *map, const void *key,
                                     uint64_t hash);
const void *hashmap_set_with_hash(struct hashmap *map, const void *item,
                                  uint64_t hash);
void hashmap_set_grow_by_power(struct hashmap *map, size_t power);
void hashmap_set_load_factor(struct hashmap *map, double load_factor);

#ifdef __cpluscplus
}
#endif

#endif
