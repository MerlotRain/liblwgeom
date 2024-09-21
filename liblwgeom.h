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

#include <stdint.h>
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
 * light-weight geometry types
 */
#define POINTTYPE      1
#define LINETYPE       2
#define POLYTYPE       3
#define MPOINTTYPE     4
#define MLINETYPE      5
#define MPOLYTYPE      6
#define COLLECTIONTYPE 7

/**
 * Global functions for memory/logging handlers.
 */
typedef void *(*lwallocator)(size_t size);
typedef void *(*lwreallocator)(void *mem, size_t size);
typedef void (*lwfreeor)(void *mem);
typedef void (*lwreporter)(const char *fmt, va_list ap) __attribute__((format(printf, 1, 0)));
typedef void (*lwdebuglogger)(int level, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));

/******************************************************************
 * LWGEOM and LWBOX both use LWFLAGS bit mask.
 * Serializations (may) use different bit mask schemes.
 */
typedef uint16_t lwflags_t;

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

/******************************************************************
 * LWGEOM Global tolerance
 */
extern double lwtolerance(double tol);
extern double lwtolerance2();

/******************************************************************
 * POINT2D, POINT3D, POINT3DM, POINT4D
 */
typedef struct {
	double x, y;
} POINT2D;

typedef struct {
	double x, y, z;
} POINT3DZ;

typedef struct {
	double x, y, z;
} POINT3D;

typedef struct {
	double x, y, m;
} POINT3DM;

typedef struct {
	double x, y, z, m;
} POINT4D;

/******************************************************************
 * LWBOX structure.
 * We include the flags (information about dimensionality),
 * so we don't have to constantly pass them
 * into functions that use the GBOX.
 */
typedef struct {
	lwflags_t flags;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
} LWBOX;

/******************************************************************
 * LWGEOM structure.
 */
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

/******************************************************************
 * LWGEOM_SDO structure.
 * It's Oracle Spataial Geometry structure.
 */
typedef struct {
	int sdo_gtype;
	int sdo_srid;
	size_t sdo_elem_count;
	int *sdo_elem_info;
	size_t sdo_ord_count;
	double *sdo_ordinates;
} LWGEOM_SDO;

typedef struct {
	LWGEOM *geom;
	size_t prop_size;
	int properties[];
} LW_SGO;

typedef struct {
	size_t cur_index;
	size_t nsgo;
	size_t nsgo_max;
	LW_SGO **sgos;
} LWGEOMREADER2;

/******************************************************************
 * LWELLIPSE structure.
 * LWELLIPSE is used to describe an ellipse or circle.
 * Before V1.0, it would be treated as a regular geometric shape and
 * temporarily not included in the unified management of the LWGEOM model,
 * while providing relevant algorithms for circles or ellipses.
 */
typedef struct {
	POINT2D center;
	double major;
	double minor;
	double azimuth;
} LWELLIPSE;

typedef enum
{
	/// Two points form a circle, and the line segment between these two
	/// points is the diameter of the circle
	LWELLIPSE_CONSTRUCT_CIRCLE_2P,
	/// Three points form a circle, and these three points are on the circle
	LWELLIPSE_CONSTRUCT_CIRCLE_3P,
	/// To construct a circle with three tangent lines, six points need to
	/// be passed in. These six points form three straight lines, which can
	/// generate 0-2 circles. They are also the inscribed circles of a
	/// triangle
	LWELLIPSE_CONSTRUCT_CIRCLE_ICT
} LWELLIPSE_CONSTRUCT_ALGORITHM;

/// @brief calculate point distance
#define LW_POINTDISTANCE(x0, y0, x1, y1) sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define LW_POINTDISTANCE2(A, B)          LW_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)
#define LW_POINT_EQUAL(A, B)             (LW_POINTDISTANCE2((A), (B)) < lwtolerance2())
#define LW_POINT_ANGLE(A)                (atan2((A).y, (A).x))
#define LW_DOUBLE_NEARES(A)              (fabs((A)) < lwtolerance2())
#define LW_DOUBLE_NEARES2(A, B)          (fabs((A) - (B)) < lwtolerance2())

#define LW_PP_X(obj, i) ((obj)->pp[(i) * (obj)->cdim])
#define LW_PP_Y(obj, i) ((obj)->pp[(i) * (obj)->cdim + 1])

#define LW_FLAG_Z          0x01
#define LW_FLAG_M          0x02
#define LW_FLAG_SHELL_RING 0x04
#define LW_FLAG_HOLE_RING  0x08

#define LWFLAGS_GET_Z(flags)          ((flags) & LW_FLAG_Z)
#define LWFLAGS_GET_M(flags)          ((flags) & LW_FLAG_M)
#define LWFLAGS_GET_SHELL_RING(flags) ((flags) & LW_FLAG_SHELL_RING)
#define LWFLAGS_GET_HOLE_RING(flags)  ((flags) & LW_FLAG_HOLE_RING)

#define LWFLAGS_SET_Z(flags, value) ((flags) = (value) ? ((flags) | LW_FLAG_Z) : ((flags) & ~LW_FLAG_Z))
#define LWFLAGS_SET_M(flags, value) ((flags) = (value) ? ((flags) | LW_FLAG_M) : ((flags) & ~LW_FLAG_M))
#define LWFLAGS_SET_SHELL_RING(flags) \
	((flags) = (value) ? ((flags) | LW_FLAG_SHELL_RING) : ((flags) & ~LW_FLAG_SHELL_RING))
#define LWFLAGS_SET_HOLE_RING(flags) \
	((flags) = (value) ? ((flags) | LW_FLAG_HOLE_RING) : ((flags) & ~LW_FLAG_HOLE_RING))

#define LW_POINTBYTESIZE(hasz, hasm) (2 + ((hasz) ? 1 : 0) + ((hasm) ? 1 : 0))

extern LWGEOM *lwgeom_point(const double *pp, LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_line(uint32_t npoints, const double *points, LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_poly(const LWGEOM *shell, uint32_t nholes, const LWGEOM **holes);
extern LWGEOM *lwgeom_create_empty_mpoint(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_mline(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_mpoly(LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_collection(uint8_t type, LWBOOLEAN hasz, LWBOOLEAN hasm);
extern LWGEOM *lwgeom_create_empty_collection2(uint8_t type, uint32_t ngeoms, LWGEOM *geoms);
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
extern int lwgeom_write_wkb(const LWGEOM *obj, int hex, char **wkb, size_t *len);
extern int lwgeom_write_ewkt(const LWGEOM *obj, char **ewkt, size_t *len);
extern int lwgeom_write_ewkb(const LWGEOM *obj, char **ewkb, size_t *len);
extern int lwgeom_write_geojson(const LWGEOM *obj, char **json, size_t *len);
extern int lwgeom_write_kml(const LWGEOM *obj, char **kml, size_t *len);
extern int lwgeom_write_gml2(const LWGEOM *obj, char **gml, size_t *len);
extern int lwgeom_write_gml3(const LWGEOM *obj, char **gml, size_t *len);

extern LWGEOM *lwgeom_read_ora(const LWGEOM_SDO sdo, int flag);
extern int lwgeom_write_ora(const LWGEOM *obj, LWGEOM_SDO *sdo);

extern double lwgeom_prop_width(const LWGEOM *obj);
extern double lwgeom_prop_height(const LWGEOM *obj);
extern double lwgeom_prop_area(const LWGEOM *obj);
extern double lwgeom_prop_length(const LWGEOM *obj);

extern void lwellipse_construct_circle(const POINT2D *p, int t, LWELLIPSE *es, int *n);
extern void lwellipse_prop_eccentricity(const LWELLIPSE ell, double *v);
extern void lwellipse_prop_area(const LWELLIPSE ell, double *v);
extern void lwellipse_prop_perimeter(const LWELLIPSE ell, double *v);
extern void lwellipse_prop_foci(const LWELLIPSE ell, POINT2D *f1, POINT2D *p2);
extern void lwellipse_prop_focus_distance(const LWELLIPSE ell, double *v);
extern LWGEOM *lwellipse_stroke(LWELLIPSE e, uint32_t param, LWBOOLEAN hasz, LWBOOLEAN hasm);

extern LWGEOM *lwgeom_clone(const LWGEOM *obj);

#ifdef __cplusplus
}
#endif

#endif /* LIBLWGEOM_H */