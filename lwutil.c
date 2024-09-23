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

#include "liblwgeom.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "lwgeom_log.h"

#define LWGEOM_DEBUG_LEVEL 1

/* Default allocators */
static void *default_allocator(size_t size);
static void default_freeor(void *mem);
static void *default_reallocator(void *mem, size_t size);
lwallocator lwalloc_var = default_allocator;
lwreallocator lwrealloc_var = default_reallocator;
lwfreeor lwfree_var = default_freeor;

/* Default reporters */
static void default_noticereporter(const char *fmt, va_list ap) __attribute__((format(printf, 1, 0)));
static void default_errorreporter(const char *fmt, va_list ap) __attribute__((format(printf, 1, 0)));
lwreporter lwnotice_var = default_noticereporter;
lwreporter lwerror_var = default_errorreporter;

/* Default logger */
static void default_debuglogger(int level, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));
lwdebuglogger lwdebug_var = default_debuglogger;

#define LW_MSG_MAXLEN 256

static char *lwgeomTypeName[] = {"Unknown",
				 "Point",
				 "LineString",
				 "Polygon",
				 "MultiPoint",
				 "MultiLineString",
				 "MultiPolygon",
				 "GeometryCollection",
				 "CircularString",
				 "CompoundCurve",
				 "CurvePolygon",
				 "MultiCurve",
				 "MultiSurface",
				 "PolyhedralSurface",
				 "Triangle",
				 "Tin"};

/*
 * Default allocators
 *
 * We include some default allocators that use malloc/free/realloc
 * along with stdout/stderr since this is the most common use case
 *
 */

static void *
default_allocator(size_t size)
{
	void *mem = malloc(size);
	return mem;
}

static void
default_freeor(void *mem)
{
	free(mem);
}

static void *
default_reallocator(void *mem, size_t size)
{
	void *ret = realloc(mem, size);
	return ret;
}

/*
 * Default lwnotice/lwerror handlers
 *
 * Since variadic functions cannot pass their parameters directly, we need
 * wrappers for these functions to convert the arguments into a va_list
 * structure.
 */

static void
default_noticereporter(const char *fmt, va_list ap)
{
	char msg[LW_MSG_MAXLEN + 1];
	vsnprintf(msg, LW_MSG_MAXLEN, fmt, ap);
	msg[LW_MSG_MAXLEN] = '\0';
	fprintf(stderr, "%s\n", msg);
}

static void
default_debuglogger(int level, const char *fmt, va_list ap)
{
	char msg[LW_MSG_MAXLEN + 1];
	if (LWGEOM_DEBUG_LEVEL >= level)
	{
		/* Space pad the debug output */
		int i;
		for (i = 0; i < level; i++)
			msg[i] = ' ';
		vsnprintf(msg + i, LW_MSG_MAXLEN - i, fmt, ap);
		msg[LW_MSG_MAXLEN] = '\0';
		fprintf(stderr, "%s\n", msg);
	}
}

static void
default_errorreporter(const char *fmt, va_list ap)
{
	char msg[LW_MSG_MAXLEN + 1];
	vsnprintf(msg, LW_MSG_MAXLEN, fmt, ap);
	msg[LW_MSG_MAXLEN] = '\0';
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

/**
 * This function is called by programs which want to set up custom handling
 * for memory management and error reporting
 *
 * Only non-NULL values change their respective handler
 */
void
lwgeom_set_handlers(lwallocator allocator,
		    lwreallocator reallocator,
		    lwfreeor freeor,
		    lwreporter errorreporter,
		    lwreporter noticereporter)
{

	if (allocator)
		lwalloc_var = allocator;
	if (reallocator)
		lwrealloc_var = reallocator;
	if (freeor)
		lwfree_var = freeor;

	if (errorreporter)
		lwerror_var = errorreporter;
	if (noticereporter)
		lwnotice_var = noticereporter;
}

void
lwgeom_set_debuglogger(lwdebuglogger debuglogger)
{

	if (debuglogger)
		lwdebug_var = debuglogger;
}

void
lwnotice(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	/* Call the supplied function */
	(*lwnotice_var)(fmt, ap);

	va_end(ap);
}

void
lwerror(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	/* Call the supplied function */
	(*lwerror_var)(fmt, ap);

	va_end(ap);
}

void
lwdebug(int level, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	/* Call the supplied function */
	(*lwdebug_var)(level, fmt, ap);

	va_end(ap);
}

const char *
lwtype_name(uint8_t type)
{
	if (type > 15)
	{
		/* assert(0); */
		return "Invalid type";
	}
	return lwgeomTypeName[(int)type];
}

void *
lwmalloc(size_t size)
{
	void *mem = lwalloc_var(size);
	return mem;
}

void *
lwmalloc0(size_t size)
{
	void *mem = lwalloc_var(size);
	memset(mem, 0, size);
	return mem;
}

void *
lwrealloc(void *mem, size_t size)
{
	return lwrealloc_var(mem, size);
}

void
lwfree(void *mem)
{
	lwfree_var(mem);
}

char *
lwstrdup(const char *a)
{
	size_t l = strlen(a) + 1;
	char *b = lwmalloc(l);
	strncpy(b, a, l);
	return b;
}

/*
 * Returns a new string which contains a maximum of maxlength characters starting
 * from startpos and finishing at endpos (0-based indexing). If the string is
 * truncated then the first or last characters are replaced by "..." as
 * appropriate.
 *
 * The caller should specify start or end truncation by setting the truncdirection
 * parameter as follows:
 *    0 - start truncation (i.e. characters are removed from the beginning)
 *    1 - end truncation (i.e. characters are removed from the end)
 */

char *
lwmessage_truncate(char *str, int startpos, int endpos, int maxlength, int truncdirection)
{
	char *output;
	char *outstart;

	/* Allocate space for new string */
	output = lwmalloc(maxlength + 4);
	output[0] = '\0';

	/* Start truncation */
	if (truncdirection == 0)
	{
		/* Calculate the start position */
		if (endpos - startpos < maxlength)
		{
			outstart = str + startpos;
			strncat(output, outstart, endpos - startpos + 1);
		}
		else
		{
			if (maxlength >= 3)
			{
				/* Add "..." prefix */
				outstart = str + endpos + 1 - maxlength + 3;
				strncat(output, "...", 4);
				strncat(output, outstart, maxlength - 3);
			}
			else
			{
				/* maxlength is too small; just output "..." */
				strncat(output, "...", 4);
			}
		}
	}

	/* End truncation */
	if (truncdirection == 1)
	{
		/* Calculate the end position */
		if (endpos - startpos < maxlength)
		{
			outstart = str + startpos;
			strncat(output, outstart, endpos - startpos + 1);
		}
		else
		{
			if (maxlength >= 3)
			{
				/* Add "..." suffix */
				outstart = str + startpos;
				strncat(output, outstart, maxlength - 3);
				strncat(output, "...", 4);
			}
			else
			{
				/* maxlength is too small; just output "..." */
				strncat(output, "...", 4);
			}
		}
	}

	return output;
}
