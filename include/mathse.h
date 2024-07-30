/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef MATHSE_H
#define MATHSE_H

#define MATHSE_VERSION_MAJOR 1
#define MATHSE_VERSION_MINOR 0
#define MATHSE_VERSION       ((MATHSE_VERSION_MAJOR * 100) + MATHSE_VERSION_MINOR)
// clang-format off
#define MATHSE_SO_VERSION 1:0:0
// clang-format on

#ifndef EXTERN
#ifdef _WIN32
/* Windows - set up dll import/export decorators. */
#if defined(DLL_EXPORT)
/* Building shared library. */
#define EXTERN __declspec(dllexport)
#elif defined(USING_SE_SHARED)
/* Using shared library. */
#define EXTERN __declspec(dllimport)
#else
/* Building static library. */
#define EXTERN /* nothing */
#endif
#elif __GNUC__ >= 4
#define EXTERN __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550) /* Sun Studio >= 8 */
#define EXTERN __global
#else
#define EXTERN /* nothing */
#endif
#endif /* EXTERN */

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define DBL_NEAR(a, b) (fabs((a) - (b)) < 4 * DBL_EPSILON)

#if defined(USE_TCMALLOC)

#include <google/tcmalloc.h>

#define malloc(size)        tc_malloc(size)
#define calloc(count, size) tc_calloc(count, size)
#define realloc(ptr, size)  tc_realloc(ptr, size)
#define free(ptr)           tc_free(ptr)

#elif defined(USE_JEMALLOC)

#include <jemalloc/jemalloc.h>

#define malloc(size)        je_malloc(size)
#define calloc(count, size) je_calloc(count, size)
#define realloc(ptr, size)  je_realloc(ptr, size)
#define free(ptr)           je_free(ptr)

#endif

#ifdef __cpluscplus
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

#ifdef __cpluscplus
}
#endif

#endif