#include "i4.h"
#include <stdlib.h>
#include <string.h>

struct __i4 {
  int size;
  int *prop;
  ptrdiff_t *ptr;
};

i4_t *create_i4(ptrdiff_t *p, size_t prop_size) {
  i4_t *i4 = (i4_t *)malloc(sizeof(i4_t));
  if (i4 == NULL)
    return NULL;
  memset(i4, 0, sizeof(i4_t));
  i4->size = prop_size;
  i4->prop = NULL;
  i4->ptr = p;
  if (prop_size > 0) {
    int *prop_ptr = (int *)malloc(sizeof(int) * prop_size);
    if (!prop_ptr) {
      free(i4);
      return NULL;
    }
    memset(prop_ptr, 0, sizeof(int) * prop_size);
    i4->prop = prop_ptr;
  }

  return i4;
}

void destroy_i4(i4_t *p) {
  if (p == NULL)
    return;
  if (p->prop)
    free(p->prop);
  if (p->ptr)
    free(p->ptr);
  free(p);
}

ptrdiff_t *i4_ptr(i4_t *p) {
  if (p == NULL)
    return NULL;
  return p->ptr;
}

void set_i4_prop(i4_t *p, size_t index, int prop) {
  if (p == NULL)
    return;

  if (index >= p->size)
    return;

  memcpy(p->prop + index * sizeof(int), &prop, sizeof(int));
}

int get_i4_prop(i4_t *p, size_t index) {
  if (p == NULL)
    return -1;

  if (index >= p->size)
    return -1;

  return *(p->prop + index * sizeof(int));
}

int i4_prop_size(i4_t *p) {
  if (p == NULL)
    return -1;
  return p->size;
}
