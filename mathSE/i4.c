#include "i4.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief The i4 object is a structure with a simple int property. It will be
 * used to obtain information such as the source of the operation result object
 * after the data flow calculation is completed.
 *
 */
struct se_i4 {
  /* The property size */
  int size;
  /* The property value pointer */
  int *prop;
  /* The object pointer */
  const void *ptr;
};

/**
 * @brief Create an i4 object and specify the number of attributes of the
 * object. Each attribute is a simple int value.
 *
 * @param p data pointer value
 * @param prop_size size of the property
 * @return i4_t*
 */
i4_t *create_i4(const void *p, size_t prop_size) {
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

/**
 * @brief Destroy the i4 object
 *
 * @param p
 */
void destroy_i4(i4_t *p) {
  if (p == NULL)
    return;
  if (p->prop)
    free(p->prop);
  if (p->ptr)
    free(p->ptr);
  free(p);
}

/**
 * @brief Get i4 properties pointer
 *
 * @param p
 * @return ptrdiff_t*
 */
const void *i4_ptr(i4_t *p) {
  if (p == NULL)
    return NULL;
  return p->ptr;
}

/**
 * @brief Set the i4 prop object
 *
 * @param p
 * @param index
 * @param prop
 */
void set_i4_prop(i4_t *p, size_t index, int prop) {
  if (p == NULL)
    return;

  if (index >= p->size)
    return;

  memcpy(p->prop + index * sizeof(int), &prop, sizeof(int));
}

/**
 * @brief Get the i4 prop object
 *
 * @param p
 * @param index
 * @return int
 */
int get_i4_prop(i4_t *p, size_t index) {
  if (p == NULL)
    return -1;

  if (index >= p->size)
    return -1;

  return *(p->prop + index * sizeof(int));
}

/**
 * @brief Get i4 properties size
 *
 * @param p
 * @return int
 */
int i4_prop_size(i4_t *p) {
  if (p == NULL)
    return -1;
  return p->size;
}
