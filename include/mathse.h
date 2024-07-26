/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/


#ifndef MATHSE_H
#define MATHSE_H

#define MATHSE_VERSION_MAJOR 1
#define MATHSE_VERSION_MINOR 0
#define MATHSE_VERSION       ((MATHSE_VERSION_MAJOR * 100) + MATHSE_VERSION_MINOR)
// clang-format off
#define MATHSE_SO_VERSION 1:0:0
// clang-format on

#ifndef EXTERN
#    ifdef _WIN32
/* Windows - set up dll import/export decorators. */
#        if defined(DLL_EXPORT)
/* Building shared library. */
#            define EXTERN __declspec(dllexport)
#        elif defined(USING_UV_SHARED)
/* Using shared library. */
#            define EXTERN __declspec(dllimport)
#        else
/* Building static library. */
#            define EXTERN /* nothing */
#        endif
#    elif __GNUC__ >= 4
#        define EXTERN __attribute__((visibility("default")))
#    elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550) /* Sun Studio >= 8 */
#        define EXTERN __global
#    else
#        define EXTERN /* nothing */
#    endif
#endif /* EXTERN */

/* The __nonnull function attribute marks pointer arguments which
   must not be NULL.  */
#if (defined(__GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 303)         \
    && !defined(__cplusplus)
#  undef __nonnull
#  define __nonnull(params) __attribute__ ((__nonnull__ params))
// cygwin/newlib has this
#  ifndef __nonnull_all
#    define __nonnull_all __attribute__ ((__nonnull__))
#  endif
#  define HAVE_NONNULL
#else
#  undef __nonnull
#  undef HAVE_NONNULL
#  define __nonnull(params)
#  define __nonnull_all
#endif


#ifndef MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(USE_TCMALLOC)

#    include <google/tcmalloc.h>

#    define malloc(size)        tc_malloc(size)
#    define calloc(count, size) tc_calloc(count, size)
#    define realloc(ptr, size)  tc_realloc(ptr, size)
#    define free(ptr)           tc_free(ptr)

#elif defined(USE_JEMALLOC)

#    include <jemalloc/jemalloc.h>

#    define malloc(size)        je_malloc(size)
#    define calloc(count, size) je_calloc(count, size)
#    define realloc(ptr, size)  je_realloc(ptr, size)
#    define free(ptr)           je_free(ptr)

#endif

/**
 * Geometric models, supporting common geometric models such as point, path,
 * ring, polyline, polygon, multipoint, etc.
 */
typedef struct SEGeom_t se_geom;

typedef struct SESpatialReference_t se_spatailReference;

typedef struct SEi4_t se_i4;

typedef struct SEDatastream_t se_datastream;

typedef struct SETable_t se_table;

typedef struct SECanvas_t se_canvas;



#define GEOMETRY_IO_TYPE_WKT     0
#define GEOMETRY_IO_TYPE_WKB     1
#define GEOMETRY_IO_TYPE_GEOJSON 2
#define GEOMETRY_IO_TYPE_EWKT    3
#define GEOMETRY_IO_TYPE_EWKB    4
#define GEOMETRY_IO_TYPE_KML     5
#define GEOMETRY_IO_TYPE_GML     6
#define GEOMETRY_IO_TYPE_GML2    7

/* Calculate the length of a geometry */
#define GEOMETRY_PROP_VALUE_LENGTH 0
/* Calculate the width of a geometry */
#define GEOMETRY_PROP_VALUE_WIDTH 1
/* Calculate the height of a geometry */
#define GEOMETRY_PROP_VALUE_HEIGHT 2
/* Calculate the area of a geometry */
#define GEOMETRY_PROP_VALUE_AREA 3

/* Get a clone of the geometry */
#define GEOMETRY_PROP_GEO_CLONE 10
/* Get the marker point of the geometry, which must be inside the geometry */
#define GEOMETRY_PROP_GEO_LABEL 11
/* Get the center point of the geometry */
#define GEOMETRY_PROP_GEO_CENTER 12
/* Get the centroid point of the geometry */
#define GEOMETRY_PROP_GEO_CENTROID 13
/* Get the envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE 14
/* Get the oriented minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ORIENTED_ENVELOPE 15
/* Get the minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE 16
/* Get the inner minimum rectangle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_RECT 17
/* Get the inner minimum square of the geometry */
#define GEOMETRY_PROP_GEO_INNER_SQUARE 18
/* Get the inner minimum circle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_CIRCLE 19
/* Get the smallest convex polygon that contains all the points int the geometry
 */
#define GEOMETRY_PROP_GEO_CONVEX_HULL 20
/* Get the simplified geometry */
#define GEOMETRY_PROP_GEO_SIMPLIFY 21
/* Geometric simplification that preserves the original geometric topology */
#define GEOMETRY_PROP_GEO_TOPOLOGY_PRESERVING_SIMPLIFY 22
/* Get the boundary of the geometry */
#define GEOMETRY_PROP_GEO_BOUNDARY 23

/* Delete all duplicate points, and pass in the duplicate point judgment
 * threshold (double) in para */
#define GEOMETRY_MODIFY_REMOVE_REPEAT 40
/* Douglas compression algorithm, para is the compression algorithm threshold
 * (double) */
#define GEOMETRY_MODIFY_DOUGLAS 41
/* Reverse the order of the points in the geometry, para is null */
#define GEOMETRY_MODIFY_REVERSE 42

/* Computes the result of union two geometries */
#define GEOMETRY_COMBINE_UNION 50
/* Computes the result of the intersection of two geometries */
#define GEOMETRY_COMBINE_INTERSECT 51
/* Computes the result of the difference of two geometries*/
#define GEOMETRY_COMBINE_DIFFERENCE 52
/* Calculate the result of the difference between two geometric symmetries */
#define GEOMETRY_COMBINE_SYM_DIFFERENCE 53

/* Two geometries do not fit this relationship */
#define GEOMETRY_RELATION_RESULT_UNFIT -1
/* The two geometries do not satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_FALSE 0
/* Two geometries satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_TRUE 1

/* Two geometries is disjoint each other */
#define GEOMETRY_SPR_IS_DISJOINT 60
/* Two geometries is intersect each other */
#define GEOMETRY_SPR_IS_INTERSECT 61
/* Two geometries is contain each other */
#define GEOMETRY_SPR_IS_CONTAIN 62
/* Two geometries is cross each other */
#define GEOMETRY_SPR_IS_CROSS 63
/* Two geometries is equal */
#define GEOMETRY_SPR_IS_EQUAL 64
/* Two geometries is touch each other */
#define GEOMETRY_SPR_IS_TOUCH 65
/* Two geometries is overlap each other */
#define GEOMETRY_SPR_IS_OVERLAP 66
/* Two geometries is within each other */
#define GEOMETRY_SPR_IS_WITHIN 67

/* Check geometry is simple */
#define GEOMETRY_CHECK_1_SIMPLE 80
/* Check geometry is close */
#define GEOMETRY_CHECK_1_CLOSE 81
/* Check geometry is good direction */
#define GEOMETRY_CHECK_1_GOOD_DIRECTION 82
/* Check geometry is self cross */
#define GEOMETRY_CHECK_1_SELF_CROSS 83
/* Check geometry has repeat point */
#define GEOMETRY_CHECK_1_REPEAT 84
/* Check line is lap */
#define GEOMETRY_CHECK_1_LAP 85

/* Check geometry repeat points */
#define GEOMETRY_CHECK_2_REPEAT_POINT 0x01
/* Check geometry isolate points */
#define GEOMETRY_CHECK_2_ISOLATE_POINT 0x02
/* Check geometry pseudo endpoints */
#define GEOMETRY_CHECK_2_PSEUDO_ENDPOINT 0x04

/**
 * @brief read the geometry from the data
 * @param data the data of the geometry
 * @param len If it is binary data, \a len represents the length of the data. If
 * it is text data, \a len can be 0.
 * @param type type of data
 */
EXTERN se_geom *geom_read(const char *data, int len, int type) __nonnull((1));

EXTERN void geome_write(const se_geom *geom, char **data, int *len, int type, int flag) __nonnull((1, 3));

EXTERN void geom_free(se_geom *geom)__nonnull((1));

EXTERN double geom_tolerance(double tol);

EXTERN double geom_prop_value(const se_geom *geom, int mode)__nonnull((1));

EXTERN se_geom *geom_prop_geo(const se_geom *geom, int mode)__nonnull((1));

EXTERN void geom_prop_geo2(const se_geom *geom, int mode, double *paras)__nonnull((1,3));

typedef struct SESpatialIndex_t se_spatialindex;

#define SPATIALINDEX_RTREE   0
#define SPAITALINDEX_MVRTREE 1
#define SPATIALINDEX_TPRTREE 2
#define SPATIALINDEX_ENCODE_BG_GRID_CODE
#define SPATIALINDEX_ENCODE_GEOHASH_CODE
#define SPATIALINDEX_PLUS_CODE

/* Apply transformation to observation - in forward or inverse direction */
/* Forward */
#define TRANS_FORWARD 1
/* do nothing */
#define TRANS_NONE 0
/* Inverse */
#define TRANS_INVERSE -1


EXTERN se_spatailReference *create_spatialreference_WKT(const char *wkt);

EXTERN se_spatailReference *create_spatialreference_EPSG(int code);

EXTERN void trans_coordinates(const mse_spatailReference *crc1,
                              const mse_spatailReference *crs2,
                              double                   *p,
                              int                       pn,
                              int                       flag);


#define CANVAS_OPERATION_DEFAULT 0

#ifdef HAVE_LIBPNG
#    define CANVAS_LIBPNG 1
#endif // HAVE_LIBPNG

#ifdef HAVE_QT
#    define CANVAS_QIMAGE 2
#endif // HAVE_QT

#ifdef __cplusplus
}
#endif

#endif