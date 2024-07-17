#ifndef __MATHSE_I4_H__
#define __MATHSE_I4_H__

#ifdef __cplusplus
extern "C" {
#endif


/* se_i4 is an attribute object, which consists of two parts, one is the object
 * pointer, and the second is the object's attribute information.
 * These attribute information are not possessed by the object itself, such as
 * polygon area, polygon enclosing rectangle, etc. Rather, they are some markup
 * attributes, such as the ID of the polygon, and the correspondence between the
 * polygon and a certain field attribute.
 * These properties are passed
 * externally, and we agree that the passed-in value can only be a simple int
 * value, and the meaning of the value is determined externally.
 * After completing a set of object operations, these attribute values
 * still exist in the object, and the object can be obtained by obtaining
 * the attribute values. */
typedef struct se_i4_t se_i4_t;

/* Create i4 */
SE_API se_i4_t *create_i4(const void *p, size_t prop_size);
/* Destroy i4 */
SE_API void destroy_i4(se_i4_t *p);
/* Return prop pointer */
SE_API const void *i4_ptr(se_i4_t *p);
/* Set prop value */
SE_API void set_i4_prop(se_i4_t *p, size_t index, int prop);
/* Get prop value */
SE_API int get_i4_prop(se_i4_t *p, size_t index);
/* Return prop size*/
SE_API int i4_prop_size(se_i4_t *p);

#ifdef __cplusplus
}
#endif

#endif//__MATHSE_I4_H__