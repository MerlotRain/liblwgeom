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

#ifndef LIBLWGEOM_H
#define LIBLWGEOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>

typedef int LWBOOLEAN;
#define LW_TRUE            1
#define LW_FALSE           0

#define LW_GEOM_POINT      1
#define LW_GEOM_LINE       2
#define LW_GEOM_POLY       3
#define LW_GEOM_MPOINT     4
#define LW_GEOM_MLINE      5
#define LW_GEOM_MPOLY      6
#define LW_GEOM_COLLECTION 7

typedef void *(*lwmalloctor)(size_t size);
typedef void *(*lwreallocator)(void *ptr, size_t size);
typedef void *(*lwcallocator)(size_t count, size_t size);
typedef void (*lwfreeor)(void *ptr);
extern int lw_replace_allocator(lwmalloctor malloctor,
                                lwreallocator reallocator,
                                lwcallocator callocator, lwfreeor freeor);

void *lwcalloc(size_t count, size_t size);
void *lwmalloc0(size_t size);
void *lwmalloc(size_t size);
void lwfree(void *ptr);
void *lwrealloc(void *ptr, size_t size);

typedef struct {
    double x, y;
} POINT2D;

typedef struct {
    double x, y, z;
} POINT3D;

typedef struct {
    double x, y, z;
} POINT3DZ;

typedef struct {
    double x, y, m;
} POINT3DM;

typedef struct {
    double x, y, z, m;
} POINT4D;

typedef struct {
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    double zmin;
    double zmax;
} LWBOX;

typedef struct LWGEOM LWGEOM; /* forward declaration */

struct LWGEOM {
    LWBOX env;        ///< geometry envelope
    uint8_t type;     ///< geometry type
    uint32_t npoints; ///< number of points
    double *pp;       ///< point pointer
    uint16_t flags;   ///< flags
    uint32_t ngeoms;  ///< number of geometries
    LWGEOM **geoms;   ///< multi objects pointer
};

extern double lwgeom_tolerance(double tol);
extern double lwgeom_tolerance2();

/// @brief calculate point distance
#define LW_POINTDISTANCE(x0, y0, x1, y1) \
    sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define LW_POINTDISTANCE2(A, B)       LW_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)

/// @brief check point A, B is equal
#define LW_POINT_EQUAL(A, B)          (LW_POINTDISTANCE2((A), (B)) < lwgeom_tolerance2())

/// @brief return point A angle
#define LW_POINT_ANGLE(A)             (atan2((A).y, (A).x))

/// @brief check double value is equal
#define LW_DOUBLE_NEARES(A)           (fabs((A)) < lwgeom_tolerance2())

/// @brief check double value is equal
#define LW_DOUBLE_NEARES2(A, B)       (fabs((A) - (B)) < lwgeom_tolerance2())

/// @brief return coordinate X in point i
#define LW_PP_X(obj, i)               ((obj)->pp[(i) * (obj)->cdim])
/// @brief return coordinate Y in point i
#define LW_PP_Y(obj, i)               ((obj)->pp[(i) * (obj)->cdim + 1])

#define LW_FLAG_Z                     0x01
#define LW_FLAG_M                     0x02
#define LW_FLAG_SHELL_RING            0x04
#define LW_FLAG_HOLE_RING             0x08

#define LWFLAGS_GET_Z(flags)          ((flags) & LW_FLAG_Z)
#define LWFLAGS_GET_M(flags)          ((flags) & LW_FLAG_M)
#define LWFLAGS_GET_SHELL_RING(flags) ((flags) & LW_FLAG_SHELL_RING)
#define LWFLAGS_GET_HOLE_RING(flags)  ((flags) & LW_FLAG_HOLE_RING)

#define LWFLAGS_SET_Z(flags, value) \
    ((flags) = (value) ? ((flags) | LW_FLAG_Z) : ((flags) & ~LW_FLAG_Z))
#define LWFLAGS_SET_M(flags, value) \
    ((flags) = (value) ? ((flags) | LW_FLAG_M) : ((flags) & ~LW_FLAG_M))
#define LWFLAGS_SET_SHELL_RING(flags)                   \
    ((flags) = (value) ? ((flags) | LW_FLAG_SHELL_RING) \
                       : ((flags) & ~LW_FLAG_SHELL_RING))
#define LWFLAGS_SET_HOLE_RING(flags)                   \
    ((flags) = (value) ? ((flags) | LW_FLAG_HOLE_RING) \
                       : ((flags) & ~LW_FLAG_HOLE_RING))

#define LW_POINTBYTESIZE(hasz, hasm) (2 + ((hasz) ? 1 : 0) + ((hasm) ? 1 : 0))

extern LWGEOM *lwgeom_point(const double *pp, LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_line(uint32_t npoints, const double *points,
                           LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_poly(const LWGEOM *shell, uint32_t nholes,
                           const LWGEOM **holes);
extern LWGEOM *lwgeom_create_empty_mpoint(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_mline(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_mpoly(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_collection(uint8_t type, LWBOOLEAN hasz,
                                              LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_collection2(uint8_t type, uint32_t ngeoms,
                                               LWGEOM *geoms);
extern LWGEOM *lwgeom_mpoint_add_point(LWGEOM *mobj, LWGEOM *obj);
extern LWGEOM *lwgeom_mline_add_line(LWGEOM *mobj, LWGEOM *obj);
extern LWGEOM *lwgeom_mpoly_add_poly(LWGEOM *mobj, LWGEOM *obj);
extern LWGEOM *lwgeom_collection_add_geom(LWGEOM *mobj, LWGEOM *obj);

extern void lwgeom_free(LWGEOM *obj);

extern int lwgeom_has_z(const LWGEOM *obj);
extern int lwgeom_has_m(const LWGEOM *obj);
extern int lwgeom_dim_coordinate(const LWGEOM *obj);
extern int lwgeom_dim_geometry(const LWGEOM *obj);
extern int lwgeom_children_count(const LWGEOM *obj);
extern LWGEOM *lwgeom_child_at(const LWGEOM *obj, int i);
extern int lwgeom_points_count(const LWGEOM *obj);
extern int lwgeom_point_at(const LWGEOM *obj, int n, double *point);
extern double *lwgeom_points(const LWGEOM *obj);

extern double lwgeom_get_x(const LWGEOM *obj, uint32_t i);
extern double lwgeom_get_y(const LWGEOM *obj, uint32_t i);
extern double lwgeom_get_z(const LWGEOM *obj, uint32_t i);
extern double lwgeom_get_m(const LWGEOM *obj, uint32_t i);

extern LWGEOM *lwgeom_read_wkt(const char *wkt, size_t len);
extern LWGEOM *lwgeom_read_wkb(const char *wkb, size_t len, int hex);
extern LWGEOM *lwgeom_read_ewkt(const char *ewkt, size_t len);
extern LWGEOM *lwgeom_read_ewkb(const char *ewkb, size_t len);
extern LWGEOM *lwgeom_read_geojson(const char *json, size_t len);
extern LWGEOM *lwgeom_read_kml(const char *kml, size_t len);
extern LWGEOM *lwgeom_read_gml2(const char *gml, size_t len);
extern LWGEOM *lwgeom_read_gml3(const char *gml, size_t len);

extern int lwgeom_write_wkt(const LWGEOM *obj, char **wkt, size_t *len);
extern int lwgeom_write_wkb(const LWGEOM *obj, int hex, char **wkb,
                            size_t *len);
extern int lwgeom_write_ewkt(const LWGEOM *obj, char **ewkt, size_t *len);
extern int lwgeom_write_ewkb(const LWGEOM *obj, char **ewkb, size_t *len);
extern int lwgeom_write_geojson(const LWGEOM *obj, char **json, size_t *len);
extern int lwgeom_write_kml(const LWGEOM *obj, char **kml, size_t *len);
extern int lwgeom_write_gml2(const LWGEOM *obj, char **gml, size_t *len);
extern int lwgeom_write_gml3(const LWGEOM *obj, char **gml, size_t *len);

typedef struct {
    int sdo_gtype;
    int sdo_srid;
    size_t sdo_elem_count;
    int *sdo_elem_info;
    size_t sdo_ord_count;
    double *sdo_ordinates;
} LWGEOM_SDO;

extern LWGEOM *lwgeom_read_ora(const LWGEOM_SDO sdo, int flag);
extern int lwgeom_write_ora(const LWGEOM *obj, LWGEOM_SDO *sdo);

extern double lwgeom_prop_width(const LWGEOM *obj);
extern double lwgeom_prop_height(const LWGEOM *obj);
extern double lwgeom_prop_area(const LWGEOM *obj);
extern double lwgeom_prop_length(const LWGEOM *obj);

#ifdef __cplusplus
}
#endif

#endif /* LIBLWGEOM_H */