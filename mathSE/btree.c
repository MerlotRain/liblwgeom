#include "btree.h"
#include <stdint.h>

struct __btree {
  btree_node *root;
  int (*key_compare)(const void *a, const void *b);
  void (*key_destroy)(const void *ptr);
  void (*value_destroy)(const void *ptr);
};

struct __btree_node {
  void *key;
  void *value;
  btree_node *left;
  btree_node *right;
  int8_t balance;
  uint8_t left_child;
  uint8_t right_child;
};
