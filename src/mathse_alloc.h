#ifndef __MATHSE_ALLOC_H__
#define __MATHSE_ALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

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

#endif //__MATHSE_ALLOC_H__