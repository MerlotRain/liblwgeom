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
#include <string.h>

LWBOX
lwgeom__query_envolpe(const double *pp, int npoints, int cdim)
{
	assert(pp);
	double xmin = pp[0];
	double xmax = pp[0];
	double ymin = pp[1];
	double ymax = pp[1];

	for (int i = 1; i < npoints; ++i)
	{
		xmin = pp[i * cdim] > xmin ? xmin : pp[i * cdim];
		xmax = pp[i * cdim] < xmax ? xmax : pp[i * cdim];
		ymin = pp[i * cdim + 1] > ymin ? ymin : pp[i * cdim + 1];
		xmax = pp[i * cdim + 1] < xmax ? xmax : pp[i * cdim + 1];
	}

	LWBOX box = {.xmin = xmin, .ymin = ymin, .xmax = xmax, .ymax = ymax, .zmin = 0.0, .zmax = 0.0};
	return box;
}

int
nv__check_single_ring(const double *pp, int npoints, int cdim)
{
	assert(pp);
	// At least 4 points are required to form a ring
	if (npoints < 4)
		return LW_FALSE;

	double x0 = pp[0];
	double y0 = pp[1];
	double xn = pp[(ptrdiff_t)(npoints * cdim)];
	double yn = pp[(ptrdiff_t)(npoints * cdim + 1)];

	return LW_DOUBLE_NEARES2(x0, xn) && LW_DOUBLE_NEARES2(y0, yn);
}

double
lwgeom_get_x(const LWGEOM *obj, uint32_t i)
{
	return 0.0;
}

double
lwgeom_get_y(const LWGEOM *obj, uint32_t i)
{
	return 0.0;
}

double
lwgeom_get_z(const LWGEOM *obj, uint32_t i)
{
	return 0.0;
}

double
lwgeom_get_m(const LWGEOM *obj, uint32_t i)
{
	return 0.0;
}

/* ---------------------------- geometry factory ---------------------------- */

LWGEOM *
lwgeom_point(const double *pp, int hasz, int hasm)
{
	assert(pp);
	LWGEOM *obj = (LWGEOM *)lwmalloc(sizeof(LWGEOM));
	if (!obj)
		return NULL;
	memset(obj, 0, sizeof(LWGEOM));
	obj->type = LW_GEOM_POINT;
	obj->npoints = 1;
	size_t msize = LW_POINTBYTESIZE(hasz, hasm) * sizeof(double);
	obj->pp = (double *)lwmalloc(msize);
	if (!obj->pp)
	{
		lwgeom_free(obj);
		return NULL;
	}
	memcpy(obj->pp, pp, msize);
	LWFLAGS_SET_Z(obj->flags, hasz);
	LWFLAGS_SET_M(obj->flags, hasm);

	obj->geoms = NULL;
	return obj;
}

LWGEOM *
lwgeom_line(uint32_t npoints, const double *points, LWBOOLEAN hasz, LWBOOLEAN hasm)
{
	return NULL;
}
LWGEOM *
lwgeom_poly(const LWGEOM *shell, uint32_t nholes, const LWGEOM **holes)
{
	return NULL;
}

LWGEOM *
lwgeom_create_empty_mpoint(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
	return NULL;
}

LWGEOM *
lwgeom_create_empty_mline(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
	return NULL;
}

LWGEOM *
lwgeom_create_empty_mpoly(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
	return NULL;
}

LWGEOM *
lwgeom_create_empty_collection(uint8_t type, LWBOOLEAN hasz, LWBOOLEAN hasm)
{
	return NULL;
}

LWGEOM *
lwgeom_create_empty_collection2(uint8_t type, uint32_t ngeoms, LWGEOM *geoms)
{
	return NULL;
}

LWGEOM *
lwgeom_mpoint_add_point(LWGEOM *mobj, LWGEOM *obj)
{
	return NULL;
}

LWGEOM *
lwgeom_mline_add_line(LWGEOM *mobj, LWGEOM *obj)
{
	return NULL;
}

LWGEOM *
lwgeom_mpoly_add_poly(LWGEOM *mobj, LWGEOM *obj)
{
	return NULL;
}
LWGEOM *
lwgeom_collection_add_geom(LWGEOM *mobj, LWGEOM *obj)
{
	return NULL;
}

int
lwgeom_has_z(const LWGEOM *obj)
{
	return 0;
}

int
lwgeom_has_m(const LWGEOM *obj)
{
	return 0;
}

int
lwgeom_dim_coordinate(const LWGEOM *obj)
{
	return 0;
}

int
lwgeom_dim_geometry(const LWGEOM *obj)
{
	return 0;
}

int
lwgeom_children_count(const LWGEOM *obj)
{
	return 0;
}
LWGEOM *
lwgeom_child_at(const LWGEOM *obj, int i)
{
	return NULL;
}

int
lwgeom_points_count(const LWGEOM *obj)
{
	return 0;
}

int
lwgeom_point_at(const LWGEOM *obj, int n, double *point)
{
	return 0;
}

double *
lwgeom_points(const LWGEOM *obj)
{
	return NULL;
}

void
lwgeom__free(LWGEOM *g)
{
	assert(g);
	if (g->flags != 0)
	{
		lwfree(g->pp);
	}
	lwfree(g);
}

/// @brief free geometry object
/// @param obj
/// @return
void
lwgeom_free(LWGEOM *obj)
{
	assert(obj);
	if (obj->ngeoms == 1)
	{
		lwgeom__free(obj);
	}
	else
	{
		for (int i = 0; i < obj->ngeoms; ++i)
		{
			LWGEOM *sub = obj->geoms[i];
			if (sub == NULL)
				continue;
			lwgeom__free(sub);
		}
	}
}
/* -------------------------------- tolerance ------------------------------- */

static double g_tolerance = 0.0001;

/// Set the tolerance used in geometric operations. This interface returns the
/// tolerance currently in use.
double
nv_tolerance(double tol)
{
	double tmp = g_tolerance;
	g_tolerance = tol;
	return tmp;
}

double
tolerence()
{
	return g_tolerance;
}

/* --------------------------- geometry algorithm --------------------------- */

/// @brief Computes whether a ring defined by a geom::CoordinateSequence is
/// oriented counter-clockwise.
/// @param pp point pointer
/// @param npoints point number
/// @param cdim coordinate dim
int
nv_ccw(const double *pp, int npoints, int cdim)
{
	if (!nv__check_single_ring(pp, npoints, cdim))
		return LW_FALSE;

	// The ring must be a convex point at the vertex extreme value, which is the
	// product of the line segments formed by the vertices before and after the
	// point. If it is greater than 0, it means the ring is counterclockwise,
	// and if it is less than 0, it means the ring is clockwise.

	// Flat Ring
	// The Y coordinate of at least one of the adjacent points of the lowest
	// point on the ring is the same as that of the lowest point.

	// of points without closing endpoint
	// Search for the highest point
	double lowY = pp[1];
	int lowIndex = 0;
	for (int i = 0; i < npoints; ++i)
	{
		double ty = pp[(ptrdiff_t)(i * cdim + 1)];
		if (ty < lowY)
		{
			lowIndex = i;
			lowY = ty;
		}
	}

	double x1 = pp[(ptrdiff_t)((lowIndex - 1) * cdim)];
	double y1 = pp[(ptrdiff_t)((lowIndex - 1) * cdim + 1)];
	double x2 = pp[(ptrdiff_t)(lowIndex * cdim)];
	double y2 = pp[(ptrdiff_t)(lowIndex * cdim + 1)];
	double x3 = pp[(ptrdiff_t)(((lowIndex + 1) % npoints) * cdim)];
	double y3 = pp[(ptrdiff_t)(((lowIndex + 1) % npoints) * cdim + 1)];

	double r = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
	return (r > 0) ? LW_TRUE : LW_FALSE;
}

/// @brief Check if the point is on the left or right side of the line
/// @param obj line object
/// @param xy point coordinates
/// @return 0: The distance from the point to the line is less than the
/// tolerance; 1: left; 2: right
int
nv_left_right(const LWGEOM *obj, double *xy)
{
	return 0;
}

/// @brief Get the convexity of a vertex on a ring object
/// @param obj single ring object
/// @param index vertex index When index is -1, get the concave and convex
/// properties of all vertices
/// @param convex convexity of the vertex
void
nv_vertex_convex(const LWGEOM *obj, int index, int *convex)
{}

static double
nv__normalize_positive(double angle)
{
	if (angle < 0.0)
	{
		while (angle < 0.0)
		{
			angle += 2 * M_PI;
		}
		// in case round-off error bumps the value over
		if (angle >= 2 * M_PI)
		{
			angle = 0.0;
		}
	}
	else
	{
		while (angle >= 2 * M_PI)
		{
			angle -= 2 * M_PI;
		}
		// in case round-off error bumps the value under
		if (angle < 0.0)
		{
			angle = 0.0;
		}
	}
	return angle;
}

static double
nv__diff(double ang1, double ang2)
{
	double delAngle;

	if (ang1 < ang2)
	{
		delAngle = ang2 - ang1;
	}
	else
	{
		delAngle = ang1 - ang2;
	}

	if (delAngle > M_PI)
	{
		delAngle = 2 * M_PI - delAngle;
	}

	return delAngle;
}

static double
nv__azimuth(const POINT2D p1, const POINT2D p2)
{
	const double dx = p2.x - p1.x;
	const double dy = p2.y - p1.y;
	return (atan2(dx, dy) * 180.0 / M_PI);
}

static int
nv__line_intersection(double p1x, double p1y, POINT2D v1, double p2x, double p2y, POINT2D v2, POINT2D *intersection)
{
	const double d = v1.y * v2.x - v1.x * v2.y;

	if (LW_DOUBLE_NEARES2(d, 0))
		return LW_FALSE;

	const double dx = p2x - p1x;
	const double dy = p2y - p1y;
	const double k = (dy * v2.x - dx * v2.y) / d;

	intersection->x = p1x + v1.x * k;
	intersection->y = p1y + v1.y * k;
	return LW_TRUE;
}

/// @brief point angle with nv_point(0, 0)
/// @param p the point to compute the angle for
/// @return the angle of the point
double
nv_angle(const POINT2D p0)
{
	return atan2(p0.y, p0.x);
}

/// @brief Returns the angle of the vector from p0 to p1, relative to the
/// positive X-axis.
/// @param p0 the first point
/// @param p1 the second point
/// @return the angle of the vector from p0 to p1
double
nv_angle2(const POINT2D p0, const POINT2D p1)
{
	double dx = p1.x - p0.x;
	double dy = p1.y - p0.y;
	return atan2(dy, dx);
}

/// @brief Tests whether the angle between p0-p1-p2 is acute.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return LW_TRUE if the angle is acute
int
nv_acute(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
	double dx0 = p0.x - p1.x;
	double dy0 = p0.y - p1.y;
	double dx1 = p2.x - p1.x;
	double dy1 = p2.y - p1.y;
	double dotprod = dx0 * dx1 + dy0 * dy1;
	return dotprod > 0;
}

/// @brief Tests whether the angle between p0-p1-p2 is obtuse.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return LW_TRUE if the angle is obtuse
int
nv_obtuse(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
	double dx0 = p0.x - p1.x;
	double dy0 = p0.y - p1.y;
	double dx1 = p2.x - p1.x;
	double dy1 = p2.y - p1.y;
	double dotprod = dx0 * dx1 + dy0 * dy1;
	return dotprod < 0;
}

/// @brief Returns the unoriented smallest angle between two vectors.
/// @param tip1 the first tip point
/// @param tail the tail point
/// @param tip2 the second tip point
/// @return the unoriented smallest angle between two vectors
double
nv_angle_between(const POINT2D tip1, const POINT2D tail, const POINT2D tip2)
{
	double a1 = nv_angle2(tail, tip1);
	double a2 = nv_angle2(tail, tip2);

	return nv__diff(a1, a2);
}

/// @brief Computes the interior angle between two segments of a ring.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return the interior angle between two segments of a ring
double
nv_interior_angle(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
	double angle_prev = nv_angle2(p1, p0);
	double angle_next = nv_angle2(p1, p2);
	return nv__normalize_positive(angle_next - angle_prev);
}

/// @brief The bisector segment of AB-CD is (point, projection of point by \a
/// angle)
/// @param A
/// @param B
/// @param C
/// @param D
/// @param p
/// @param angle
/// @return
int
nv_angle_bisector(const POINT2D A, const POINT2D B, const POINT2D C, const POINT2D D, POINT2D *p, double *angle)
{

	*angle = (nv__azimuth(A, B) + nv__azimuth(C, D)) / 2.0;

	int intersection = LW_FALSE;
	nv_segment_intersection(A, B, C, D, p, &intersection);
	return intersection;
}

/// @brief Computes the distance from a point p to a line segment AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
double
nv_dis_point_to_segment(const POINT2D p, const POINT2D A, const POINT2D B)
{
	if (LW_DOUBLE_NEARES2(A.x, B.x) && LW_DOUBLE_NEARES2(A.y, B.y))
	{
		return LW_POINTDISTANCE2(p, A);
	}

	/*
	    (1)
			    AC dot AB
			r = ---------
			    ||AB||^2

	    r has the following meaning:
	    r=0 P = A
	    r=1 P = B
	    r<0 P is on the backward extension of AB
	    r>1 P is on the forward extension of AB
	    0<r<1 P is interior to AB
	*/

	double r = ((p.x - A.x) * (B.x - A.x) + (p.y - A.y) * (B.y - A.y)) /
		   ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));

	if (r <= 0.0)
	{
		return LW_POINTDISTANCE2(p, A);
	}
	if (r >= 1.0)
	{
		return LW_POINTDISTANCE2(p, B);
	}

	/*
	    (2)
		    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
		s = -----------------------------
				L^2

	    Then the distance from C to P = |s|*L.
	*/

	double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y)) /
		   ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));

	return fabs(s) * sqrt(((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y)));
	return 0;
}

/// @brief Computes the perpendicular distance from a point p to the (infinite)
/// line containing the points AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
double
nv_dis_point_to_perpendicular(const POINT2D p, const POINT2D A, const POINT2D B)
{
	/*
		    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
		s = -----------------------------
				     L^2

		Then the distance from C to P = |s|*L.
	*/

	double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y)) /
		   ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
	return fabs(s) * sqrt(((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y)));
}

void
nv_segment_intersection(const POINT2D p1,
			const POINT2D p2,
			const POINT2D p3,
			const POINT2D p4,
			POINT2D *pin,
			int *intersection)
{
	*intersection = LW_FALSE;
	double vl = LW_POINTDISTANCE2(p1, p2);
	double wl = LW_POINTDISTANCE2(p3, p4);

	if (LW_DOUBLE_NEARES2(vl, 0.0) || LW_DOUBLE_NEARES2(wl, 0.0))
		return;

	POINT2D v = {.x = (p2.x - p1.x) / vl, .y = (p2.y - p1.y) / vl};
	POINT2D w = {.x = (p4.x - p3.x) / wl, .y = (p4.y - p3.y) / wl};

	if (!nv__line_intersection(p1.x, p1.y, v, p3.x, p3.y, w, pin))
	{
		return;
	}

	*intersection = LW_TRUE;
}