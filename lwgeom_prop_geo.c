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

#include "liblwgeom_internel.h"
#include <float.h>
#include <math.h>
#include <assert.h>

/// check box is null?
#define MV_BOX_NULL(B) \
	(isnan(B.xmin) && isnan(B.xmax) && isnan(B.ymin) && isnan(B.ymax)) || \
	    (LW_DOUBLE_NEARES2(B.xmin, DBL_MAX) && LW_DOUBLE_NEARES2(B.ymin, DBL_MAX) && \
	     LW_DOUBLE_NEARES2(B.xmax, -DBL_MAX) && LW_DOUBLE_NEARES2(B.ymax, -DBL_MAX))

/// init box
#define MV_BOX_INIT(B) \
	LWBOX B = { \
	    .xmin = DBL_MIN, .ymin = DBL_MIN, .zmin = DBL_MIN, .xmax = DBL_MAX, .ymax = DBL_MAX, .zmax = DBL_MAX};

/// @brief Check two box is intersects
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return LW_TRUE if the two Envelopes intersect
int
nv_box_intersects(const LWBOX env1, const LWBOX env2)
{
	const double x1 = LWMAX(env1.xmin, env2.xmin);
	const double x2 = LWMIN(env1.xmax, env2.xmax);
	if (x1 > x2)
		return LW_FALSE;
	const double y1 = LWMAX(env1.ymin, env2.ymin);
	const double y2 = LWMIN(env1.ymax, env2.ymax);
	return y1 <= y2;
}

/// @brief Computes the intersection of two Envelopes
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return the intersection of the two Envelopes
LWBOX
nv_box_intersection(const LWBOX env1, const LWBOX env2)
{
	MV_BOX_INIT(b)
	if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2))
	{
		return b;
	}

	if (nv_box_intersects(env1, env2))
	{
		b.xmin = env1.xmin > env2.xmin ? env1.xmin : env2.xmin;
		b.ymin = env1.ymin > env2.ymin ? env1.ymin : env2.ymin;
		b.xmax = env1.xmax < env2.xmax ? env1.xmax : env2.xmax;
		b.ymax = env1.ymax < env2.ymax ? env1.ymax : env2.ymax;
	}
	return b;
}

/// @brief Enlarges the boundary of the Envelope so that it contains
/// @param env1 the original Envelope
/// @param env2 the Envelope to be contained
/// @return the enlarged Envelope
LWBOX
nv_box_union(const LWBOX env1, const LWBOX env2)
{
	MV_BOX_INIT(b)
	if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2))
	{
		return b;
	}
	b.xmin = LWMIN(env1.xmin, env2.xmin);
	b.ymin = LWMIN(env1.ymin, env2.ymin);
	b.xmax = LWMIN(env1.xmax, env2.xmax);
	b.ymax = LWMIN(env1.ymax, env2.ymax);
	return b;
}

/// @brief Convert nv_box object to LWGEOM
/// @param e the nv_box object
/// @param gdim the geometry dimension
/// @return the LWGEOM object
LWGEOM *
nv_box_stroke(LWBOX e, int gdim)
{
	return NULL; // TODO
}

#undef MV_BOX_NULL
#undef MV_BOX_INIT

static int
nv__arc_center(POINT2D c0, POINT2D c1, POINT2D c2, POINT2D *center)
{
	if (c0.x == c2.x && c0.y == c2.y)
	{
		center->x = (c0.x + c1.x) / 2.0;
		center->y = (c0.y + c1.y) / 2.0;
		return 1;
	}

	POINT2D a = {.x = c1.x - c2.x, .y = c1.y - c2.y};
	POINT2D b = {.x = c2.x - c0.x, .y = c2.y - c0.y};
	POINT2D c = {.x = c0.x - c1.x, .y = c0.y - c1.y};

	double d1 = -(b.x * c.x + b.y * c.y);
	double d2 = -(c.x * a.x + c.y * a.y);
	double d3 = -(a.x * b.x + a.y * b.y);

	double e1 = d2 * d3;
	double e2 = d3 * d1;
	double e3 = d1 * d2;
	double e = e1 + e2 + e3;

	POINT2D G3 = {.x = c0.x + c1.x + c2.x, .y = c0.y + c1.y + c2.y};
	POINT2D H = {.x = (e1 * c0.x + e2 * c1.x + e3 * c2.x) / e, .y = (e1 * c0.y + e2 * c1.y + e3 * c2.y) / e};

	center->x = 0.5 * (G3.x - H.x);
	center->y = 0.5 * (G3.y - H.y);
	return 1;
}

LWGEOM *
lwgeom_clone(const LWGEOM *obj)
{
	assert(obj);

	LWGEOM *clone = (LWGEOM *)lwmalloc(sizeof(LWGEOM));
	if (clone == NULL)
	{
		return NULL;
	}

	clone->ngeoms = obj->ngeoms;
	if (obj->ngeoms == 1) {}
	return NULL;
}