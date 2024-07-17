#include "btree.h"
#include <stdint.h>

struct se_btree {
  btree_node_t *root;
  int (*key_compare)(const void *a, const void *b);
  void (*key_destroy)(const void *ptr);
  void (*value_destroy)(const void *ptr);
};

struct se_btree_node {
  void *key;
  void *value;
  btree_node_t *left;
  btree_node_t *right;
  int8_t balance;
  uint8_t left_child;
  uint8_t right_child;
};
