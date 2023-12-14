#ifndef I4_H
#define I4_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __i4 i4_t;

i4_t *create_i4(ptrdiff_t *p, size_t prop_size);
void destroy_i4(i4_t *p);
ptrdiff_t *i4_ptr(i4_t *p);
void set_i4_prop(i4_t *p, size_t index, int prop);
int get_i4_prop(i4_t *p, size_t index);
int i4_prop_size(i4_t *p);

#ifdef __cplusplus
}
#endif

#endif // I4_H