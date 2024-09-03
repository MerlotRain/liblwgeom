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

#include "geom-internal.h"
#include <string.h>

struct nv_box nv__query_envolpe(const double *pp, int npoints, int cdim)
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

    struct nv_box box = {.min = {.x = xmin, .y = ymin},
                         .max = {.x = xmax, .y = ymax}};
    return box;
}

int nv__check_single_ring(const double *pp, int npoints, int cdim)
{
    assert(pp);
    // At least 4 points are required to form a ring
    if (npoints < 4)
        return NV_FALSE;

    double x0 = pp[0];
    double y0 = pp[1];
    double xn = pp[(ptrdiff_t)(npoints * cdim)];
    double yn = pp[(ptrdiff_t)(npoints * cdim + 1)];

    return NV_DOUBLE_NEARES2(x0, xn) && NV_DOUBLE_NEARES2(y0, yn);
}

double nv__geo_get_x(const struct nv_geom *obj, uint32_t i)
{
    return 0.0;
}

double nv__geo_get_y(const struct nv_geom *obj, uint32_t i)
{
    return 0.0;
}

double nv__geo_get_z(const struct nv_geom *obj, uint32_t i)
{
    return 0.0;
}

double nv__geo_get_m(const struct nv_geom *obj, uint32_t i)
{
    return 0.0;
}

/* ---------------------------- geometry factory ---------------------------- */

struct nv_geom *nv_geo_point(const double *pp, int hasz, int hasm)
{
    assert(pp);
    struct nv_geom *obj = (struct nv_geom *)nv__malloc(sizeof(struct nv_geom));
    if (!obj)
        return NULL;
    memset(obj, 0, sizeof(struct nv_geom));
    obj->type = NV_GEOM_POINT;
    obj->npoints = 1;
    size_t msize = NV_POINTBYTESIZE(hasz, hasm) * sizeof(double);
    obj->pp = (double *)nv__malloc(msize);
    if (!obj->pp) {
        nv_geo_free(obj);
        return NULL;
    }
    memcpy(obj->pp, pp, msize);
    NV_FLAGS_SET_Z(obj->flags, hasz);
    NV_FLAGS_SET_M(obj->flags, hasm);

    obj->objects = NULL;
    return obj;
}

struct nv_geom *nv_geo_line(uint32_t npoints, const double *points, int hasz,
                            int hasm)
{
    return NULL;
}
struct nv_geom *nv_geo_poly(const struct nv_geom *shell, uint32_t nholes,
                            const struct nv_geom **holes)
{
    return NULL;
}

struct nv_geom *nv_geo_create_empty_mpoint(char hasz, char hasm)
{
    return NULL;
}

struct nv_geom *nv_geo_create_empty_mline(char hasz, char hasm)
{
    return NULL;
}

struct nv_geom *nv_geo_create_empty_mpoly(char hasz, char hasm)
{
    return NULL;
}

struct nv_geom *nv_geo_create_empty_collection(uint8_t type, char hasz,
                                               char hasm)
{
    return NULL;
}

struct nv_geom *nv_geo_create_empty_collection2(uint8_t type, uint32_t ngeoms,
                                                struct nv_geom *geoms)
{
    return NULL;
}

struct nv_geom *nv_geo_mpoint_add_point(struct nv_geom *mobj,
                                        struct nv_geom *obj)
{
    return NULL;
}

struct nv_geom *nv_geo_mline_add_line(struct nv_geom *mobj, struct nv_geom *obj)
{
    return NULL;
}

struct nv_geom *nv_geo_mpoly_add_poly(struct nv_geom *mobj, struct nv_geom *obj)
{
    return NULL;
}
struct nv_geom *nv_geo_collection_add_geom(struct nv_geom *mobj,
                                           struct nv_geom *obj)
{
    return NULL;
}

int nv_geo_hasz(const struct nv_geom *obj)
{
    return 0;
}

int nv_geo_hasm(const struct nv_geom *obj)
{
    return 0;
}

int nv_geo_dim_c(const struct nv_geom *obj)
{
    return 0;
}

int nv_geo_dim_g(const struct nv_geom *obj)
{
    return 0;
}

int nv_geo_sub_n(const struct nv_geom *obj)
{
    return 0;
}
struct nv_geom *nv_geo_sub_at(const struct nv_geom *obj, int i)
{
    return NULL;
}

int nv_geo_point_n(const struct nv_geom *obj)
{
    return 0;
}

int nv_geo_point_at(const struct nv_geom *obj, int n, double *point)
{
    return 0;
}

double *nv_geo_points(const struct nv_geom *obj)
{
    return NULL;
}

void nv__geo_free(struct nv_geom *g)
{
    assert(g);
    if (g->flags != 0) {
        nv__free(g->pp);
    }
    nv__free(g);
}

/// @brief free geometry object
/// @param obj
/// @return
void nv_geo_free(struct nv_geom *obj)
{
    assert(obj);
    return;
    //    if (g->ngeoms == 1) {
    //        nv__geo_free(g);
    //    }
    //    else {
    //        for (int i = 0; i < g->ngeoms; ++i) {
    //            struct nv_geom *sub = g->objects[i];
    //            if (sub == NULL)
    //                continue;
    //            nv__geo_free(sub);
    //        }
    //    }
}

/* ------------------------------- geometry io ------------------------------ */

struct nv_geom *nv_geo_read(int flag, const char *data, size_t len)
{
    assert(data);
    switch (flag) {
    case NV_GEOMETRY_IO_WKT: {
        return nv__geo_read_wkt(data, len);
    }
    case NV_GEOMETRY_IO_WKB: {
        return nv__geo_read_wkb(data, len, NV_FALSE);
    }
    case NV_GEOMETRY_IO_WKB_HEX: {
        return nv__geo_read_wkb(data, len, NV_TRUE);
    }
    case NV_GEOMETRY_IO_GEOJSON: {
        return nv__geo_read_geojson(data, len);
    }
    case NV_GEOMETRY_IO_EWKT: {
        return nv__geo_read_ewkt(data, len);
    }
    case NV_GEOMETRY_IO_EWKB: {
        return nv__geo_read_ewkb(data, len);
    }
    case NV_GEOMETRY_IO_KML: {
        return nv__geo_read_kml(data, len);
    }
    case NV_GEOMETRY_IO_GML2: {
        return nv__geo_read_gml2(data, len);
    }
    case NV_GEOMETRY_IO_GML3: {
        return nv__geo_read_gml3(data, len);
    }
    default:
        break;
    }
    return NULL;
}

int nv_geo_write(int flag, const struct nv_geom *obj, char **data, size_t *len)
{
    return 0;
}

/* ------------------------- geometry reader writer ------------------------- */

struct nv_geom *nv_i4_object(struct nv_i4 *i4)
{
    assert(i4);
    return i4->obj;
}

void nv_i4_propProp(const struct nv_i4 *i4, int *propSize, int **prop)
{
    assert(i4);
    *propSize = i4->propSize;
    *prop = i4->prop;
}

int nv_i4_prop_value(const struct nv_i4 *i4, size_t index)
{
    assert(i4);
    assert(index < i4->propSize);
    return i4->prop[index];
}

struct nv_geo_reader2 *nv_geo_reader2_new(size_t size)
{
    if (size == 0)
        size = 10;
    struct nv_geo_reader2 *reader =
        (struct nv_geo_reader2 *)nv__malloc(sizeof(struct nv_geo_reader2));
    if (reader == NULL)
        return NULL;
    reader->size = size;
    reader->objs = (struct nv_i4 **)nv__calloc(size, sizeof(struct nv_i4 *));
    if (reader->objs == NULL) {
        nv__free(reader);
        return NULL;
    }
    reader->current = 0;
    reader->index = NULL;

    return reader;
}

void nv_free_reader2(struct nv_geo_reader2 *reader)
{
}

void nv_input_reader(struct nv_geo_reader2 *reader, const struct nv_geom *obj,
                     int propSize, int *prop)
{
}

struct nv_i4 *nv_geo_reader2_iterator(struct nv_geo_reader2 *writer)
{
    return NULL;
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
        return NV_FALSE;

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
    return (r > 0) ? NV_TRUE : NV_FALSE;
}

/// Get the value property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_VALUE_*
double nv_prop_value(const struct nv_geom *obj, int mode)
{
    assert(obj);
    switch (mode) {
    case NV_GEOMETRY_PROP_VALUE_LENGTH: {
        return nv_prop_length_value(obj);
    }
    case NV_GEOMETRY_PROP_VALUE_WIDTH: {
        return nv_prop_width_value(obj);
    }
    case NV_GEOMETRY_PROP_VALUE_HEIGHT: {
        return nv_prop_height_value(obj);
    }
    case NV_GEOMETRY_PROP_VALUE_AREA: {
        return nv_prop_area_value(obj);
    }
    default:
        return 0;
    }
}

/// Get the geometric object property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_GEO_*, except
/// GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE and GEOMETRY_PROP_GEO_INNER_CIRCLE.
struct nv_geom *nv_prop_geo(const struct nv_geom *obj, int mode)
{
    assert(obj);
    switch (mode) {
    case NV_GEOMETRY_PROP_GEO_CLONE: {
        return NULL;
    }
    }
    return NULL;
}

void nv_prop_geo2(const struct nv_geom *obj, int mode, double *paras)
{
}

/// @brief Check if the point is on the left or right side of the line
/// @param obj line object
/// @param xy point coordinates
/// @return 0: The distance from the point to the line is less than the
/// tolerance; 1: left; 2: right
int nv_left_right(const struct nv_geom *obj, double *xy)
{
    return 0;
}

/// @brief Get the convexity of a vertex on a ring object
/// @param obj single ring object
/// @param index vertex index When index is -1, get the concave and convex
/// properties of all vertices
/// @param convex convexity of the vertex
void nv_vertex_convex(const struct nv_geom *obj, int index, int *convex)
{
}
