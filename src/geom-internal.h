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

#ifndef NV_GEOINTERNAL_H
#define NV_GEOINTERNAL_H

#include "nv-common.h"

#define NV_FLAG_Z                      0x01
#define NV_FLAG_M                      0x02
#define NV_FLAG_SHELL_RING             0x04
#define NV_FLAG_HOLE_RING              0x08

#define NV_FLAGS_GET_Z(flags)          ((flags) & NV_FLAG_Z)
#define NV_FLAGS_GET_M(flags)          ((flags) & NV_FLAG_M)
#define NV_FLAGS_GET_SHELL_RING(flags) ((flags) & NV_FLAG_SHELL_RING)
#define NV_FLAGS_GET_HOLE_RING(flags)  ((flags) & NV_FLAG_HOLE_RING)

#define NV_FLAGS_SET_Z(flags, value) \
    ((flags) = (value) ? ((flags) | NV_FLAG_Z) : ((flags) & ~NV_FLAG_Z))
#define NV_FLAGS_SET_M(flags, value) \
    ((flags) = (value) ? ((flags) | NV_FLAG_M) : ((flags) & ~NV_FLAG_M))
#define NV_FLAGS_SET_SHELL_RING(flags)                  \
    ((flags) = (value) ? ((flags) | NV_FLAG_SHELL_RING) \
                       : ((flags) & ~NV_FLAG_SHELL_RING))
#define NV_FLAGS_SET_HOLE_RING(flags)                  \
    ((flags) = (value) ? ((flags) | NV_FLAG_HOLE_RING) \
                       : ((flags) & ~NV_FLAG_HOLE_RING))

#define NV_POINTBYTESIZE(hasz, hasm) (2 + ((hasz) ? 1 : 0) + ((hasm) ? 1 : 0))

/* ---------------------------- mg kernel struct ---------------------------- */

/// It is a geometric description that is consistent with sdo-geometry in the
/// coordinate dimension, and any constructed geometry does not have an M value,
/// even if nv_order is exported to other common interchange formats (v1.0).
///
/// It will be used to record single geometry and multi geometry, where multi
/// geometry is an array of single geometry, and we will not make any special
/// descriptions of the geometry itself, such as inner and outer loops.
/// Normally, we consider the ring you input to be correct, or generally
/// correct, and at least it will not exhibit self intersection or other
/// situations.
struct nv_geom {
    struct nv_box env;        ///< geometry envelope
    uint8_t type;             ///< geometry type
    uint32_t npoints;         ///< number of points
    double *pp;               ///< point pointer
    uint16_t flags;           ///< flags
    uint32_t ngeoms;          ///< number of geometries
    struct nv_geom **objects; ///< multi objects pointer
};

/// @brief create integer attribute object
struct nv_i4 {
    int id;              ///< i4 object id
    struct nv_geom *obj; ///< geometry object
    int propSize;        ///< property size
    int *prop;           ///< property array
};

struct nv_geo_reader2 {
    int current;         ///< current index
    size_t size;         ///< size of object array
    struct nv_i4 **objs; ///< object array
    struct rtree *index; ///< spatial index
};

/// @brief tolerance value
extern double tolerence();

/// @brief tolerance value macro
#define NV_TOLERANCE tolerence()

/// @brief calculate point distance
#define NV_POINTDISTANCE(x0, y0, x1, y1) \
    sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define NV_POINTDISTANCE2(A, B) NV_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)

/// @brief check point A, B is equal
#define NV_POINT_EQUAL(A, B)    (NV_POINTDISTANCE2((A), (B)) < NV_TOLERANCE)

/// @brief return point A angle
#define NV_POINT_ANGLE(A)       (atan2((A).y, (A).x))

/// @brief check double value is equal
#define NV_DOUBLE_NEARES(A)     (fabs((A)) < NV_TOLERANCE)

/// @brief check double value is equal
#define NV_DOUBLE_NEARES2(A, B) (fabs((A) - (B)) < NV_TOLERANCE)

/// @brief return coordinate X in point i
#define NV_PP_X(obj, i)         ((obj)->pp[(i) * (obj)->cdim])
/// @brief return coordinate Y in point i
#define NV_PP_Y(obj, i)         ((obj)->pp[(i) * (obj)->cdim + 1])

struct nv_box nv__query_envolpe(const double *pp, int npoints, int cdim);
int nv__check_single_ring(const double *pp, int npoints, int cdim);

/* ----------------------- geometry IO extern function ---------------------- */

double nv__geo_get_x(const struct nv_geom *obj, uint32_t i);
double nv__geo_get_y(const struct nv_geom *obj, uint32_t i);
double nv__geo_get_z(const struct nv_geom *obj, uint32_t i);
double nv__geo_get_m(const struct nv_geom *obj, uint32_t i);

struct nv_geom *nv__geo_read_wkt(const char *wkt, size_t len);
struct nv_geom *nv__geo_read_wkb(const char *wkb, size_t len, int hex);
struct nv_geom *nv__geo_read_ewkt(const char *ewkt, size_t len);
struct nv_geom *nv__geo_read_ewkb(const char *ewkb, size_t len);
struct nv_geom *nv__geo_read_geojson(const char *json, size_t len);
struct nv_geom *nv__geo_read_kml(const char *kml, size_t len);
struct nv_geom *nv__geo_read_gml2(const char *gml, size_t len);
struct nv_geom *nv__geo_read_gml3(const char *gml, size_t len);

int nv__geo_write_wkt(const struct nv_geom *obj, char **wkt, size_t *len);
int nv__geo_write_wkb(const struct nv_geom *obj, int hex, char **wkb,
                      size_t *len);
int nv__geo_write_ewkt(const struct nv_geom *obj, char **ewkt, size_t *len);
int nv__geo_write_ewkb(const struct nv_geom *obj, char **ewkb, size_t *len);
int nv__geo_write_geojson(const struct nv_geom *obj, char **json, size_t *len);
int nv__geo_write_kml(const struct nv_geom *obj, char **kml, size_t *len);
int nv__geo_write_gml2(const struct nv_geom *obj, char **gml, size_t *len);
int nv__geo_write_gml3(const struct nv_geom *obj, char **gml, size_t *len);

/* ------------------- geometry algorithm extern function ------------------- */

double nv_prop_length_value(const struct nv_geom *obj);
double nv_prop_area_value(const struct nv_geom *obj);
double nv_prop_width_value(const struct nv_geom *obj);
double nv_prop_height_value(const struct nv_geom *obj);

struct nv_geom *nv_prop_geo_clone(const struct nv_geom *obj);
void nv_prop_geo_centriod(const struct nv_geom *obj, double *xy);

struct nv_geom *nv_prop_geo_simpily_douglaspeucker(const struct nv_geom *obj);

#endif /* NV_GEOINTERNAL_H */