#ifndef __MATHSE_GEOM_H__
#define __MATHSE_GEOM_H__

#include <mathse_export.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
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
} GeometryType;

typedef enum {
    DONTCARE = -3,
    /* Dimension value of non-empty geometries (= {P, L, A}) */
    TRUE = -2,
    /* Dimension value of empty geometry (-1) */
    FALSE = -1,
    /* Dimension value of point geometry (0) */
    P = 0,
    /* Dimension value of line geometry (1) */
    L = 1,
    /* Dimension value of area geometry (2) */
    A = 2,
} GeometryDimension;


/* Segmentation tolerance as maximum angle or maximum difference between
 * approximation and circle */
typedef enum {
    /* Maximum angle between generating radii (lines from arc center to output
    * vertices) */
    MAX_ANGLE,
    /* Maximum distance between an arbitrary point on the original curve and closest
    * point on its approximation. */
    MAX_DIFFERENCE
} SegmentToleranceType;

/* --------------------------- Simple OGRGeometry --------------------------- */

/* point */
typedef struct geom_point_t {
    double x;
    double y;
} geom_point_t;

/* multi point */
typedef struct geom_mpoint_t {
    int     num;
    double *coord;
} geom_mpoint_t;

/* linestring */
typedef struct geom_linestring_t {
    int     num;
    double *coord;
} geom_linestring_t;

/* linearring */
typedef struct geom_linearring_t {
    int     num;
    double *coord;
} geom_linearring_t;

/* multi linestring */
typedef struct geom_mlinestring_t {
    int                 num;
    geom_linestring_t **geometries;
} geom_mlinestring_t;

/* polygon */
typedef struct geom_polygon_t {
    geom_linearring_t  *shell;
    int                 hole_num;
    geom_linearring_t **holes;
} geom_polygon_t;

/* multi polygon */
typedef struct geom_mpolygon_t {
    int              num;
    geom_polygon_t **geometries;
} geom_mpolygon_t;

/* -------------------------- Simple Math Geometry -------------------------- */

/* arc */
typedef struct geom_arc_t {
    geom_point_t begin;
    geom_point_t mid;
    geom_point_t end;
} geom_arc_t;

/* rectangle */
typedef struct geom_envelope_t {
    double minx;
    double maxx;
    double miny;
    double maxy;
} geom_envelope_t;

/* bezier curve */
typedef struct geom_bezier_t {
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
    double x4;
    double y4;
} geom_bezier_t;

/* line */
typedef struct geom_line_t {
    double x1;
    double y1;
    double x2;
    double y2;
} geom_line_t;

typedef struct geom_ellipse_t {
    geom_point_t center;
    double       semiMajorAxis;
    double       semiMinorAxis;
    double       azimuth;
} geom_ellipse_t;

/* triangle */
typedef struct geom_triangle_t {
    geom_point_t p1;
    geom_point_t p2;
    geom_point_t p3;
} geom_triangle_t;

/* ------------------------------ Geometry Base ----------------------------- */

/* coordinate */
typedef struct geom_coordinate_t {
    int geom_id;
    int SRID;
    union {
        geom_point_t       *point;
        geom_linestring_t  *linestring;
        geom_linearring_t  *linearring;
        geom_polygon_t     *polygon;
        geom_mpoint_t      *mpoint;
        geom_mlinestring_t *mlinestring;
        geom_mpolygon_t    *mpolygon;
    } oti;
} geom_coordinate_t;


/* --------------------------- Coordinate Factory --------------------------- */

/* create point. */
SE_API geom_coordinate_t *create_point(const double *p);


/* convert geom_lint_t to geom_coordinate_t. */
SE_API geom_coordinate_t *convert_line_to_coordinate(const geom_line_t *p);

SE_API geom_coordinate_t *convert_arc_to_coordinate(const geom_arc_t *p);

SE_API geom_coordinate_t *convert_envelope_to_coordinate(const geom_envelope_t* p);

SE_API geom_coordinate_t *convert_ellipse_to_coordinate(const geom_ellipse_t* p, bool fill);

SE_API geom_coordinate_t *convert_triangle_to_coordinate(const geom_triangle_t* p);

/* create arc line by three point */
SE_API geom_coordinate_t *create_arc1(const double *p);

/* create arc by circle and sweep angle */
SE_API geom_coordinate_t *create_arc2(const double *c,
                                      const double  radius,
                                      const double  angle1,
                                      const double  angle2);

/* create arc by begin and end point, chord */
SE_API geom_coordinate_t *create_arc3(const double *p, const double chord);

/* create single path */
SE_API geom_coordinate_t *create_linestring(const double *p, const int num);

/* create envelope */
SE_API geom_coordinate_t *create_envelope(const double *p);

#ifdef __cplusplus
}
#endif

#endif//__MATHSE_GEOM_H__