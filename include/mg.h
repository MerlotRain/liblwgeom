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
struct mg_object;

/// create a single geometry object
/// @param gdim geometry dimension 0:point, 1:line, 2:area
/// @param pn point number
/// @param cdim coordinate dimension 2:2D, 3:3D
/// @param pp point coordinates
/// @param flag geometry type 0: reference pp, 1: copy pp
EXTERN struct mg_object *mg_create_single(int gdim, int pn, int cdim,
                                          const double *pp, int flag);

/// create a multi geometry object
///
/// When multiple geometries of different dimensions are passed in, only
/// geometries with the same dimension as \a gdim will be accepted. When the
/// result of the combination is only one geometry, it will degenerate into a
/// simple geometry.
///
/// @param gdim geometry dimension 0:point, 1:line, 2:area
/// @param snum geometry number
/// @param subs geometry array
/// @return geometry object
EXTERN struct mg_object *mg_create_multi(int gdim, int snum,
                                         struct mg_object **subs);

/// @brief free geometry object
/// @param obj
/// @return
EXTERN void mg_free_object(struct mg_object *obj);

/// @brief return geometry coordinate dimension
/// @param obj
/// @return 2 or 3
EXTERN int mg_dim_c(const struct mg_object *obj);

/// @brief return geometry dimension
/// @param obj
/// @return 0:point, 1:line, 2:area
/// TODO: 3:collection
EXTERN int mg_dim_g(const struct mg_object *obj);

/// @brief return geometry number
/// @param obj
/// @return geometry number
EXTERN int mg_sub_n(const struct mg_object *obj);

/// @brief return sub of multi geometry
/// @param obj multi geometry
/// @param i index
/// @return sub geometry, When \a obj is a single geometry object, returns
/// itself
EXTERN struct mg_object *mg_sub_at(const struct mg_object *obj, int i);

/// @brief return point number
/// @param obj
/// @return point number
EXTERN int mg_point_n(const struct mg_object *obj);

/* --------------------------------- Factory -------------------------------- */

/// geometry IO type wkt
#define GEOMETRY_IO_WKT     0
/// geometry IO type wkb
#define GEOMETRY_IO_WKB     1
/// geometry IO type wkb hex
#define GEOMETRY_IO_WKB_HEX 2
/// geometry IO type geojson
#define GEOMETRY_IO_GEOJSON 3
/// geometry IO type ewkt
#define GEOMETRY_IO_EWKT    4
/// geometry IO type ewkb
#define GEOMETRY_IO_EWKB    5
/// geometry IO type kml
#define GEOMETRY_IO_KML     6
/// geometry IO type gml2
#define GEOMETRY_IO_GML2    7
/// geometry IO type gml3
#define GEOMETRY_IO_GML3    8

EXTERN struct mg_object *mg_read(int flag, const char *data, size_t len);

EXTERN int mg_write(int flag, const struct mg_object *obj, char **data,
                    size_t *len);

struct sdo_geometry {
    int sdo_gtype;
    int sdo_srid;
    int sdo_elem_count;
    int *sdo_elem_info;
    int sdo_ord_count;
    double *sdo_ordinates;
};

EXTERN struct mg_object *mg_read_ora(const struct sdo_geometry sdo, int flag);

EXTERN int mg_write_ora(const struct mg_object *obj, struct sdo_geometry *sdo);

/* ------------------------- geometry reader writer ------------------------- */

struct mg_i4;

struct mg_reader2;

EXTERN struct mg_object *mg_i4_object(struct mg_i4 *i4);

EXTERN void mg_i4_propProp(const struct mg_i4 *i4, int *propSize, int **prop);

EXTERN int mg_i4_prop_value(const struct mg_i4 *i4, size_t index);

EXTERN struct mg_reader2 *mg_reader2_new(size_t size);

EXTERN void mg_free_reader2(struct mg_reader2 *reader);

EXTERN void mg_reader2_push(struct mg_reader2 *reader,
                            const struct mg_object *obj, int propSize,
                            int *prop);

EXTERN struct mg_i4 *mg_reader2_iterator(struct mg_reader2 *writer);

/* --------------------------- geometry algorithm --------------------------- */

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

/// Set the tolerance used in geometric operations. This interface returns the
/// tolerance currently in use.
EXTERN double mg_tolerance(double tol);

/// Get the value property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_VALUE_*
EXTERN double mg_prop_value(const struct mg_object *obj, int mode);

/// Get the geometric object property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_GEO_*, except
/// GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE and GEOMETRY_PROP_GEO_INNER_CIRCLE.
EXTERN struct mg_object *mg_prop_geo(const struct mg_object *obj, int mode);

EXTERN void mg_prop_geo2(const struct mg_object *obj, int mode, double *paras);

/// @brief Check if the point is on the left or right side of the line
/// @param obj line object
/// @param xy point coordinates
/// @return 0: The distance from the point to the line is less than the
/// tolerance; 1: left; 2: right
EXTERN int mg_left_right(const struct mg_object *obj, double *xy);

/// @brief Computes whether a ring defined by a geom::CoordinateSequence is
/// oriented counter-clockwise.
/// @param pp point pointer
/// @param npoints point number
/// @param cdim coordinate dim
EXTERN bool mg_ccw(const double *pp, int npoints, int cdim);

/// @brief Get the convexity of a vertex on a ring object
/// @param obj single ring object
/// @param index vertex index When index is -1, get the concave and convex
/// properties of all vertices
/// @param convex convexity of the vertex
EXTERN void mg_vertex_convex(const struct mg_object *obj, int index,
                             int *convex);

/// @brief Make the rings formed by the building outlines extracted from the
/// city image rectangular
/// @param xy building outline coordinates, Points may be added or deleted
/// within the algorithm
/// @param np number of points
EXTERN void mg_building_regularization(double *xy, int np);

/// @brief K-means clustering
///
/// W only records the i4 attributes of P that conform to the classification.
/// For example, if the i4 attribute of each object is the object ID, then the
/// attribute in W is the ID of the classified object. Therefore, when calling
/// this interface, the i4 attribute in P should be identifiable.
///
/// @param P input data
/// @param n number of clusters
/// @param W output data
EXTERN void mg_kmeans(struct mg_reader2 *P, int n, struct mg_reader2 **W);

#ifdef __cplusplus
}
#endif

#endif