#ifndef __MMALLOC_H__
#define __MMALLOC_H__

#if defined(USE_TCMALLOC)

#include <google/tcmalloc.h>

#define malloc(size) tc_malloc(size)
#define calloc(count, size) tc_calloc(count, size)
#define realloc(ptr, size) tc_realloc(ptr, size)
#define free(ptr) tc_free(ptr)

#elif defined(USE_JEMALLOC)

#include <jemalloc/jemalloc.h>

#define malloc(size) je_malloc(size)
#define calloc(count, size) je_calloc(count, size)
#define realloc(ptr, size) je_realloc(ptr, size)
#define free(ptr) je_free(ptr)

#endif

#endif //__MMALLOC_H__