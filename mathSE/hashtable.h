#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hashtab_node {
  void *key;
  void *datum;
  struct hashtab_node *next;
};

struct hash_table {
  struct hashtab_node **htable;
  uint32_t size;
  uint32_t nel;
  uint32_t (*hash)(struct hash_table *, void *);
  int32_t (*compare)(struct hash_table *, void *key1, void *key2);
};

struct hash_table *hashtab_create(uint32_t (*hash)(struct hash_table *, void *),
                                  int32_t (*compare)(struct hash_table *,
                                                     void *key1, void *key2),
                                  int32_t size);
int hashtab_insert(struct hash_table *ht, void *key, void *datum);
void *hashtab_search(struct hash_table *ht, void *key);
void hashtab_free(struct hash_table *ht);

#ifdef __cplusplus
}
#endif

#endif // __HASHTABLE_H__