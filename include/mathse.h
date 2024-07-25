#ifndef __MATHSE_H__
#define __MATHSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_MATHSE_SHARED)
    /* Building shared library. */
#   define EXTERN __declspec(dllexport)
# elif defined(USING_UV_SHARED)
    /* Using shared library. */
#   define EXTERN __declspec(dllimport)
# else
    /* Building static library. */
#   define EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define EXTERN __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550) /* Sun Studio >= 8 */
# define EXTERN __global
#else
# define EXTERN /* nothing */
#endif
#endif /* EXTERN */


#ifndef MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
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


typedef struct SEGeom_t SEGeom;

enum GeometryType {
    /* Invalid geometry type */
    GEOMETRY_TYPE_NONE = -1,
    /// a point
    GEOMETRY_TYPE_POINT,
    /// a linestring
    GEOMETRY_TYPE_LINESTRING,
    /// a linear ring (linestring with 1st point == last point)
    GEOMETRY_TYPE_LINEARRING,
    /// a polygon
    GEOMETRY_TYPE_POLYGON,
    /// a collection of points
    GEOMETRY_TYPE_MULTIPOINT,
    /// a collection of linestrings
    GEOMETRY_TYPE_MULTI_LINESTRING,
    /// a collection of polygons
    GEOMETRY_TYPE_MULTI_POLYGON,
    /// a collection of heterogenous geometries
    GEOMETRY_TYPE_GEOMETRY_COLLECTION
};

EXTERN SEGeom * ora_create(int i_n, const int* i_p, int c_n, const double* c_p);
EXTERN SEGeom * SE_create_point(const double *p);
EXTERN SEGeom * SE_create_linestring(const double *p, int pn);
EXTERN SEGeom * SE_create_linearring(const double *p, int pn);
EXTERN SEGeom * SE_create_polygon(const double** pp, int* ppn, int pn);
EXTERN SEGeom * SE_create_multipoint(const double *p, int pn);
EXTERN SEGeom * SE_create_geometrycollection(int t, const SEGeom** geoms, int gn);


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

#ifdef __cplusplus
}
#endif

#endif //__MATHSE_H__