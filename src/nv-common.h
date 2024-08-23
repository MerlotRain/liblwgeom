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

#ifndef NV_COMMON_H
#define NV_COMMON_H

#include <stdio.h>
#include <nv.h>

/// It is a geometric description that is consistent with sdo-geometry in the
/// coordinate dimension, and any constructed geometry does not have an M value,
/// even if mg_order is exported to other common interchange formats (v1.0).
///
/// It will be used to record single geometry and multi geometry, where multi
/// geometry is an array of single geometry, and we will not make any special
/// descriptions of the geometry itself, such as inner and outer loops.
/// Normally, we consider the ring you input to be correct, or generally
/// correct, and at least it will not exhibit self intersection or other
/// situations.
struct nv_geobject {
    int ngeoms;    ///< number of geometries
    int gdim;      ///< geometry dimension 0:point, 1:line, 2:area
    int cdim;      ///< coordinate dimension 2:2D, 3:3D
    int npoints;   ///< number of points
    int flag;      ///< flag 0: reference input, 1: memory copy
    int clockwise; ///< 1: clockwise, 0: counterclockwise
    double *pp;    ///< point pointer
    struct nv_geobject **objects; ///< multi objects pointer
};

/// @brief create integer attribute object
struct nv_i4 {
    int id;                  ///< i4 object id
    struct nv_geobject *obj; ///< geometry object
    int propSize;            ///< property size
    int *prop;               ///< property array
};

struct nv_reader2 {
    int current;         ///< current index
    size_t size;         ///< size of object array
    struct nv_i4 **objs; ///< object array
    struct rtree *index; ///< spatial index
};

void nv_record_error(int code, const char *fmt, ...);

/// @brief tolerance value
extern double tolerence();

/// @brief tolerance value macro
#define MG_TOLERANCE tolerence()

/// @brief calculate point distance
#define MG_POINTDISTANCE(x0, y0, x1, y1) \
    sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define MG_POINTDISTANCE2(A, B) MG_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)

/// @brief check point A, B is equal
#define MG_POINT_EQUAL(A, B)    (MG_POINTDISTANCE2((A), (B)) < MG_TOLERANCE)

/// @brief return point A angle
#define MG_POINT_ANGLE(A)       (atan2((A).y, (A).x))

/// @brief check double value is equal
#define MG_DOUBLE_NEARES(A)     (fabs((A)) < MG_TOLERANCE)

/// @brief check double value is equal
#define MG_DOUBLE_NEARES2(A, B) (fabs((A) - (B)) < MG_TOLERANCE)

/* ---------------------------- mg kernel struct ---------------------------- */

/* ----------------------- geometry IO extern function ---------------------- */

struct nv_geobject *mg_read_wkt(const char *wkt, size_t len);
struct nv_geobject *mg_read_wkb(const char *wkb, size_t len, bool hex);
struct nv_geobject *mg_read_ewkt(const char *ewkt, size_t len);
struct nv_geobject *mg_read_ewkb(const char *ewkb, size_t len);
struct nv_geobject *mg_read_geojson(const char *json, size_t len);
struct nv_geobject *mg_read_kml(const char *kml, size_t len);
struct nv_geobject *mg_read_gml2(const char *gml, size_t len);
struct nv_geobject *mg_read_gml3(const char *gml, size_t len);

int mg_write_wkt(const struct nv_geobject *obj, char **wkt, size_t *len);
int mg_write_wkb(const struct nv_geobject *obj, bool hex, char **wkb,
                 size_t *len);
int mg_write_ewkt(const struct nv_geobject *obj, char **ewkt, size_t *len);
int mg_write_ewkb(const struct nv_geobject *obj, char **ewkb, size_t *len);
int mg_write_geojson(const struct nv_geobject *obj, char **json, size_t *len);
int mg_write_kml(const struct nv_geobject *obj, char **kml, size_t *len);
int mg_write_gml2(const struct nv_geobject *obj, char **gml, size_t *len);
int mg_write_gml3(const struct nv_geobject *obj, char **gml, size_t *len);

/* ------------------- geometry algorithm extern function ------------------- */

bool mg_check_single_ring(const double *pp, int npoints, int cdim);

double mg_prop_length_value(const struct nv_geobject *obj);
double mg_prop_area_value(const struct nv_geobject *obj);
double mg_prop_width_value(const struct nv_geobject *obj);
double mg_prop_height_value(const struct nv_geobject *obj);

struct nv_geobject *mg_prop_geo_clone(const struct nv_geobject *obj);
void mg_prop_geo_centriod(const struct nv_geobject *obj, double *xy);

struct nv_geobject *
mg_prop_geo_simpily_douglaspeucker(const struct nv_geobject *obj);

#endif /* NV_COMMON_H */
