/* mathSE is an algorithm library for processing two-dimensional geometric and
 * geographical information. It provides basic geometric construction, geometric
 * algorithms, spatial indexing, data storage and data display.
 */

#ifndef __MATHSE_H__
#define __MATHSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SE_EXTERN
#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_MATHSE_SHARED)
    /* Building shared library. */
#   define SE_EXTERN __declspec(dllexport)
# elif defined(USING_UV_SHARED)
    /* Using shared library. */
#   define SE_EXTERN __declspec(dllimport)
# else
    /* Building static library. */
#   define SE_EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define SE_EXTERN __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550) /* Sun Studio >= 8 */
# define SE_EXTERN __global
#else
# define SE_EXTERN /* nothing */
#endif
#endif /* SE_EXTERN */


#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if defined(USE_TCMALLOC)

#    include <google/tcmalloc.h>

#    define malloc(size)        tc_malloc(size)
#    define calloc(count, size) tc_calloc(count, size)
#    define realloc(ptr, size)  tc_realloc(ptr, size)
#    define free(ptr)           tc_free(ptr)

#elif defined(USE_JEMALLOC)

#    include <jemalloc/jemalloc.h>

#    define malloc(size)        je_malloc(size)
#    define calloc(count, size) je_calloc(count, size)
#    define realloc(ptr, size)  je_realloc(ptr, size)
#    define free(ptr)           je_free(ptr)

#endif

#ifdef __cplusplus
}
#endif

#endif //__MATHSE_H__