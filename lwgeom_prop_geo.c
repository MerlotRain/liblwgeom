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

/// check box is null?
#define MV_BOX_NULL(B) \
	(isnan(B.min.x) && isnan(B.max.x) && isnan(B.min.y) && isnan(B.max.y)) || \
	    (LW_DOUBLE_NEARES2(B.min.x, DBL_MAX) && LW_DOUBLE_NEARES2(B.min.y, DBL_MAX) && \
	     LW_DOUBLE_NEARES2(B.max.x, -DBL_MAX) && LW_DOUBLE_NEARES2(B.max.y, -DBL_MAX))

/// init box
#define MV_BOX_INIT(B) struct nv_box B = {.min = {.x = DBL_MIN, .y = DBL_MIN}, .max = {.x = DBL_MAX, .y = DBL_MAX}};

/// @brief Check two box is intersects
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return LW_TRUE if the two Envelopes intersect
int
nv_box_intersects(const struct nv_box env1, const struct nv_box env2)
{
	const double x1 = LWMAX(env1.min.x, env2.min.x);
	const double x2 = LWMIN(env1.max.x, env2.max.x);
	if (x1 > x2)
		return LW_FALSE;
	const double y1 = LWMAX(env1.min.y, env2.min.y);
	const double y2 = LWMIN(env1.max.y, env2.max.y);
	return y1 <= y2;
}

/// @brief Computes the intersection of two Envelopes
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return the intersection of the two Envelopes
struct nv_box
nv_box_intersection(const struct nv_box env1, const struct nv_box env2)
{
	MV_BOX_INIT(b)
	if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2))
	{
		return b;
	}

	if (nv_box_intersects(env1, env2))
	{
		b.min.x = env1.min.x > env2.min.x ? env1.min.x : env2.min.x;
		b.min.y = env1.min.y > env2.min.y ? env1.min.y : env2.min.y;
		b.max.x = env1.max.x < env2.max.x ? env1.max.x : env2.max.x;
		b.max.y = env1.max.y < env2.max.y ? env1.max.y : env2.max.y;
	}
	return b;
}

/// @brief Enlarges the boundary of the Envelope so that it contains
/// @param env1 the original Envelope
/// @param env2 the Envelope to be contained
/// @return the enlarged Envelope
struct nv_box
nv_box_union(const struct nv_box env1, const struct nv_box env2)
{
	MV_BOX_INIT(b)
	if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2))
	{
		return b;
	}
	b.min.x = LWMIN(env1.min.x, env2.min.x);
	b.min.y = LWMIN(env1.min.y, env2.min.y);
	b.max.x = LWMIN(env1.max.x, env2.max.x);
	b.max.y = LWMIN(env1.max.y, env2.max.y);
	return b;
}

/// @brief Convert nv_box object to nv_geom
/// @param e the nv_box object
/// @param gdim the geometry dimension
/// @return the nv_geom object
LWGEOM *
nv_box_stroke(struct nv_box e, int gdim)
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

/// @brief stroke arc to nv_geom
/// @param arc arc
/// @param maxAngleStepSizeDegress max angle step size
/// @return nv_geom
LWGEOM *
nv_arc_stroke(struct nv_arc arc, double maxAngleStepSizeDegress)
{
	double startAngle, alongAngle, endAngle;
	double cx, cy, radius;
	return 0;
}

LWGEOM *
nv_prop_geo_clone(const LWGEOM *obj)
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