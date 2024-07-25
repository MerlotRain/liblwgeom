#ifndef __MATHSE_GEOM_H__
#define __MATHSE_GEOM_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/* rectangle */
typedef struct geom_envelope_t {
    double minx;
    double maxx;
    double miny;
    double maxy;
} geom_envelope_t;

typedef struct geom_circle_t {
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};

/* ------------------------------ Geometry Base ----------------------------- */

/* coordinate */
typedef struct SEGeom_t {
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
    } tio;
} se_geom;

#ifdef __cplusplus
}
#endif

#endif //__MATHSE_GEOM_H__