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
#include "liblwgeom_internel.h"
#include <string.h>

LWBOX lwgeom__query_envolpe(const double *pp, int npoints, int cdim)
{
    assert(pp);
    double xmin = pp[0];
    double xmax = pp[0];
    double ymin = pp[1];
    double ymax = pp[1];

    for (int i = 1; i < npoints; ++i) {
        xmin = pp[i * cdim] > xmin ? xmin : pp[i * cdim];
        xmax = pp[i * cdim] < xmax ? xmax : pp[i * cdim];
        ymin = pp[i * cdim + 1] > ymin ? ymin : pp[i * cdim + 1];
        xmax = pp[i * cdim + 1] < xmax ? xmax : pp[i * cdim + 1];
    }

    LWBOX box = {.xmin = xmin,
                 .ymin = ymin,
                 .xmax = xmax,
                 .ymax = ymax,
                 .zmin = 0.0,
                 .zmax = 0.0};
    return box;
}

int nv__check_single_ring(const double *pp, int npoints, int cdim)
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

double lwgeom_get_x(const LWGEOM *obj, uint32_t i)
{
    return 0.0;
}

double lwgeom_get_y(const LWGEOM *obj, uint32_t i)
{
    return 0.0;
}

double lwgeom_get_z(const LWGEOM *obj, uint32_t i)
{
    return 0.0;
}

double lwgeom_get_m(const LWGEOM *obj, uint32_t i)
{
    return 0.0;
}

/* ---------------------------- geometry factory ---------------------------- */

LWGEOM *lwgeom_point(const double *pp, int hasz, int hasm)
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
    if (!obj->pp) {
        lwgeom_free(obj);
        return NULL;
    }
    memcpy(obj->pp, pp, msize);
    LWFLAGS_SET_Z(obj->flags, hasz);
    LWFLAGS_SET_M(obj->flags, hasm);

    obj->geoms = NULL;
    return obj;
}

LWGEOM *lwgeom_line(uint32_t npoints, const double *points, LWBOOLEAN hasz,
                    LWBOOLEAN hasm)
{
    return NULL;
}
LWGEOM *lwgeom_poly(const LWGEOM *shell, uint32_t nholes, const LWGEOM **holes)
{
    return NULL;
}

LWGEOM *lwgeom_create_empty_mpoint(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
    return NULL;
}

LWGEOM *lwgeom_create_empty_mline(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
    return NULL;
}

LWGEOM *lwgeom_create_empty_mpoly(LWBOOLEAN hasz, LWBOOLEAN hasm)
{
    return NULL;
}

LWGEOM *lwgeom_create_empty_collection(uint8_t type, LWBOOLEAN hasz,
                                       LWBOOLEAN hasm)
{
    return NULL;
}

LWGEOM *lwgeom_create_empty_collection2(uint8_t type, uint32_t ngeoms,
                                        LWGEOM *geoms)
{
    return NULL;
}

LWGEOM *lwgeom_mpoint_add_point(LWGEOM *mobj, LWGEOM *obj)
{
    return NULL;
}

LWGEOM *lwgeom_mline_add_line(LWGEOM *mobj, LWGEOM *obj)
{
    return NULL;
}

LWGEOM *lwgeom_mpoly_add_poly(LWGEOM *mobj, LWGEOM *obj)
{
    return NULL;
}
LWGEOM *lwgeom_collection_add_geom(LWGEOM *mobj, LWGEOM *obj)
{
    return NULL;
}

int lwgeom_has_z(const LWGEOM *obj)
{
    return 0;
}

int lwgeom_has_m(const LWGEOM *obj)
{
    return 0;
}

int lwgeom_dim_coordinate(const LWGEOM *obj)
{
    return 0;
}

int lwgeom_dim_geometry(const LWGEOM *obj)
{
    return 0;
}

int lwgeom_children_count(const LWGEOM *obj)
{
    return 0;
}
LWGEOM *lwgeom_child_at(const LWGEOM *obj, int i)
{
    return NULL;
}

int lwgeom_points_count(const LWGEOM *obj)
{
    return 0;
}

int lwgeom_point_at(const LWGEOM *obj, int n, double *point)
{
    return 0;
}

double *lwgeom_points(const LWGEOM *obj)
{
    return NULL;
}

void lwgeom__free(LWGEOM *g)
{
    assert(g);
    if (g->flags != 0) {
        lwfree(g->pp);
    }
    lwfree(g);
}

/// @brief free geometry object
/// @param obj
/// @return
void lwgeom_free(LWGEOM *obj)
{
    assert(obj);
    if (obj->ngeoms == 1) {
        lwgeom__free(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
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
double nv_tolerance(double tol)
{
    double tmp = g_tolerance;
    g_tolerance = tol;
    return tmp;
}

double tolerence()
{
    return g_tolerance;
}

/* --------------------------- geometry algorithm --------------------------- */

/// @brief Computes whether a ring defined by a geom::CoordinateSequence is
/// oriented counter-clockwise.
/// @param pp point pointer
/// @param npoints point number
/// @param cdim coordinate dim
int nv_ccw(const double *pp, int npoints, int cdim)
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
    for (int i = 0; i < npoints; ++i) {
        double ty = pp[(ptrdiff_t)(i * cdim + 1)];
        if (ty < lowY) {
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
int nv_left_right(const LWGEOM *obj, double *xy)
{
    return 0;
}

/// @brief Get the convexity of a vertex on a ring object
/// @param obj single ring object
/// @param index vertex index When index is -1, get the concave and convex
/// properties of all vertices
/// @param convex convexity of the vertex
void nv_vertex_convex(const LWGEOM *obj, int index, int *convex)
{
}
