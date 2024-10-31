/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef LWUTIL_H
#define LWUTIL_H

#include <stddef.h>
#include <stdarg.h>

/**
 * Return types for functions with status returns.
 */
typedef int LWBOOLEAN;
#define LW_TRUE    1
#define LW_FALSE   0
#define LW_UNKNOWN 2
#define LW_FAILURE 0
#define LW_SUCCESS 1

/**
 * Global functions for memory/logging handlers.
 */
typedef void *(*lwallocator)(size_t size);
typedef void *(*lwreallocator)(void *mem, size_t size);
typedef void (*lwfreeor)(void *mem);
typedef void (*lwreporter)(const char *fmt, va_list ap) __attribute__((format(printf, 1, 0)));
typedef void (*lwdebuglogger)(int level, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));

/**
 * Install custom memory management and error handling functions you want your
 * application to use.
 * @ingroup system
 * @todo take a structure ?
 */
extern void lwgeom_set_handlers(lwallocator allocator,
				lwreallocator reallocator,
				lwfreeor freeor,
				lwreporter errorreporter,
				lwreporter noticereporter);

extern void lwgeom_set_debuglogger(lwdebuglogger debuglogger);

/* Memory management */
void *lwcalloc(size_t count, size_t size);
void *lwmalloc0(size_t size);
void *lwmalloc(size_t size);
void lwfree(void *mem);
void *lwrealloc(void *mem, size_t size);
void init_log();

#endif /* LWUTIL_H */