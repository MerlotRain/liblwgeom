#ifndef __I4_H__
#define __I4_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __i4 i4_t;

i4_t *create_i4(const void *p, size_t prop_size);
void destroy_i4(i4_t *p);
const void *i4_ptr(i4_t *p);
void set_i4_prop(i4_t *p, size_t index, int prop);
int get_i4_prop(i4_t *p, size_t index);
int i4_prop_size(i4_t *p);

#ifdef __cplusplus
}
#endif

#endif // __I4_H__