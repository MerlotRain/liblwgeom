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

#ifndef MG_H
#define MG_H

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mg_object;

EXTERN struct mg_object *geom_create_single(int gdim, int pn, int cdim, const double* pp, int flag);

EXTERN struct mg_object *geom_create_multi(int dim, int snum, struct mg_object** subs);

EXTERN void geom_free(struct mg_object* obj);

EXTERN int geom_dim_c(const struct mg_object* obj);

EXTERN int geom_dim_g(const struct mg_object* obj);

EXTERN int geom_sub_n(const struct mg_object* obj);

EXTERN struct geom_object* mg_sub_at(const struct mg_object *obj, int i);

EXTERN int geom_point_n(const struct mg_object *p);

EXTERN struct mg_object *geom_read_wkt(const char *data, int len);

EXTERN struct mg_object *geom_read_wkb(const char *data, int len);

EXTERN struct mg_object *geom_read_geojson(const char *data, int len);

EXTERN struct mg_object *geom_read_ewkt(const char *data, int len);

EXTERN struct mg_object *geom_read_ewkb(const char *data, int len);

EXTERN struct mg_object *geom_read_kml(const char *data, int len);

EXTERN struct mg_object *geom_read_gml(const char *data, int len);

EXTERN struct mg_object *geom_read_gml2(const char *data, int len);

EXTERN struct mg_object *geom_read_ora(int i_n,const int *i_p,int c_n,int c_dim,const double *c_p);



EXTERN int geom_write_wkt(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_wkb(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_geojson(const struct mg_object *obj, char *data, int len);

EXTERN int geom_write_ewkt(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_ewkb(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_kml(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_gml(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_gml2(const struct mg_object *obj, char **data, int len);

EXTERN int geom_write_ora(struct mg_object* obj, int* i_n, int** i_p, int* c_n, int* c_dim, double **c_p);

/* Calculate the length of a geometry */
#define GEOMETRY_PROP_VALUE_LENGTH                     0
/* Calculate the width of a geometry */
#define GEOMETRY_PROP_VALUE_WIDTH                      1
/* Calculate the height of a geometry */
#define GEOMETRY_PROP_VALUE_HEIGHT                     2
/* Calculate the area of a geometry */
#define GEOMETRY_PROP_VALUE_AREA                       3

/* Get a clone of the geometry */
#define GEOMETRY_PROP_GEO_CLONE                        10
/* Get the marker point of the geometry, which must be inside the geometry */
#define GEOMETRY_PROP_GEO_LABEL                        11
/* Get the center point of the geometry */
#define GEOMETRY_PROP_GEO_CENTER                       12
/* Get the centroid point of the geometry */
#define GEOMETRY_PROP_GEO_CENTROID                     13
/* Get the envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE                     14
/* Get the oriented minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ORIENTED_ENVELOPE            15
/* Get the minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE              16
/* Get the inner minimum rectangle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_RECT                   17
/* Get the inner minimum square of the geometry */
#define GEOMETRY_PROP_GEO_INNER_SQUARE                 18
/* Get the inner minimum circle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_CIRCLE                 19
/* Get the smallest convex polygon that contains all the points int the
 * geometry
 */
#define GEOMETRY_PROP_GEO_CONVEX_HULL                  20
/* Get the simplified geometry */
#define GEOMETRY_PROP_GEO_SIMPLIFY                     21
/* Geometric simplification that preserves the original geometric topology */
#define GEOMETRY_PROP_GEO_TOPOLOGY_PRESERVING_SIMPLIFY 22
/* Get the boundary of the geometry */
#define GEOMETRY_PROP_GEO_BOUNDARY                     23

/* Delete all duplicate points, and pass in the duplicate point judgment
 * threshold (double) in para */
#define GEOMETRY_MODIFY_REMOVE_REPEAT                  40
/* Douglas compression algorithm, para is the compression algorithm threshold
 * (double) */
#define GEOMETRY_MODIFY_DOUGLAS                        41
/* Reverse the order of the points in the geometry, para is null */
#define GEOMETRY_MODIFY_REVERSE                        42

/* Computes the result of union two geometries */
#define GEOMETRY_COMBINE_UNION                         50
/* Computes the result of the intersection of two geometries */
#define GEOMETRY_COMBINE_INTERSECT                     51
/* Computes the result of the difference of two geometries*/
#define GEOMETRY_COMBINE_DIFFERENCE                    52
/* Calculate the result of the difference between two geometric symmetries */
#define GEOMETRY_COMBINE_SYM_DIFFERENCE                53

/* Two geometries do not fit this relationship */
#define GEOMETRY_RELATION_RESULT_UNFIT                 (-1)
/* The two geometries do not satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_FALSE                 0
/* Two geometries satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_TRUE                  1

/* Two geometries is disjoint each other */
#define GEOMETRY_SPR_IS_DISJOINT                       60
/* Two geometries is intersect each other */
#define GEOMETRY_SPR_IS_INTERSECT                      61
/* Two geometries is contain each other */
#define GEOMETRY_SPR_IS_CONTAIN                        62
/* Two geometries is cross each other */
#define GEOMETRY_SPR_IS_CROSS                          63
/* Two geometries is equal */
#define GEOMETRY_SPR_IS_EQUAL                          64
/* Two geometries is touch each other */
#define GEOMETRY_SPR_IS_TOUCH                          65
/* Two geometries is overlap each other */
#define GEOMETRY_SPR_IS_OVERLAP                        66
/* Two geometries is within each other */
#define GEOMETRY_SPR_IS_WITHIN                         67

/* Check geometry is simple */
#define GEOMETRY_CHECK_1_SIMPLE                        80
/* Check geometry is close */
#define GEOMETRY_CHECK_1_CLOSE                         81
/* Check geometry is good direction */
#define GEOMETRY_CHECK_1_GOOD_DIRECTION                82
/* Check geometry is self cross */
#define GEOMETRY_CHECK_1_SELF_CROSS                    83
/* Check geometry has repeat point */
#define GEOMETRY_CHECK_1_REPEAT                        84
/* Check line is lap */
#define GEOMETRY_CHECK_1_LAP                           85

/* Check geometry repeat points */
#define GEOMETRY_CHECK_2_REPEAT_POINT                  0x01
/* Check geometry isolate points */
#define GEOMETRY_CHECK_2_ISOLATE_POINT                 0x02
/* Check geometry pseudo endpoints */
#define GEOMETRY_CHECK_2_PSEUDO_ENDPOINT               0x04

EXTERN double geom_tolerance(double tol);

EXTERN double geom_prop_value(const struct mg_object *obj, int mode);

EXTERN struct mg_object *geom_prop_geo(const struct mg_object *obj, int mode);

EXTERN void geom_prop_geo2(const struct mg_object *obj, int mode, double *paras);

#ifdef __cplusplus
}
#endif

#endif