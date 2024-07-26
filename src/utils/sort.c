#include "sort.h"
#include <alloca.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct qsort_param {
  size_t s;
  size_t var;
  int (*compare)(const void *, const void *);
  void *args;
  char *t;
};

static void quick_sort_r(void *b, size_t n, size_t s,
                         int (*compare)(const void *, const void *),
                         void *arg) {

  size_t size = n * s;
  char *tmp = NULL;
  struct qsort_param p;

  if (s > 32)
    size = 2 * n * sizeof(ptrdiff_t *) + s;

  if (size < 1024) {
    p.t = (char *)alloca(size);
  } else {
    tmp = (char *)malloc(size);
    if (tmp != NULL) {
      p.t = tmp;
    }
  }
}

void quick_sort(const void *p, size_t ele_count, size_t ele_size,
                int (*compare)(const void *, const void *), void *user_data) {
  quick_sort_r((void *)p, ele_count, ele_size, compare, user_data);
}


#ifdef __cplusplus
}
#endif