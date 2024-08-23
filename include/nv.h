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

#ifndef NV_H
#define NV_H

#ifdef __cpluscplus
extern "C" {
#endif

#ifndef NV_EXTERN
#ifdef _WIN32
#if defined(BUILD_NV_SHARED)
#define NV_EXTERN __declspec(dllexport)
#elif defined(USING_NV_SHARED)
#define NV_EXTERN __declspec(dllimport)
#else
/* Building static library. */
#define NV_EXTERN /* nothing */
#endif
#elif __GNUC__ >= 4
#define NV_EXTERN __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550) /* Sun Studio >= 8 */
#define NV_EXTERN __global
#else
#define NV_EXTERN /* nothing */
#endif
#endif /* NV_EXTERN */

#include "nv/version.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef NV_FALLTHROUGH
#if (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 700) && \
    !defined(__INTEL_COMPILER)
#define NV_FALLTHROUGH __attribute__((fallthrough))
#else
#define NV_FALLTHROUGH
#endif
#endif

#ifndef NV_MAX
#define NV_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef NV_MIN
#define NV_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define NV_MAX3(a, b, c) \
    ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

#define NV_MIN3(a, b, c) \
    ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

#define NV_DBL_NEAR(a, b) (fabs((a) - (b)) < 4 * DBL_EPSILON)

NV_EXTERN unsigned int nv_version(void);
NV_EXTERN const char *nv_version_string(void);

typedef void *(*nv_malloc_func)(size_t size);
typedef void *(*nv_realloc_func)(void *ptr, size_t size);
typedef void *(*nv_calloc_func)(size_t count, size_t size);
typedef void (*nv_free_func)(void *ptr);

NV_EXTERN int uv_replace_allocator(nv_malloc_func malloc_func,
                                   nv_realloc_func realloc_func,
                                   nv_calloc_func calloc_func,
                                   nv_free_func free_func);

NV_EXTERN int nv_error_code();

NV_EXTERN char *nv_error_messgae();

/* ---------------------------------- Point --------------------------------- */

struct nv_point {
    double x;
    double y;
};

/// @brief point angle with nv_point(0, 0)
/// @param p the point to compute the angle for
/// @return the angle of the point
NV_EXTERN double nv_angle(const struct nv_point p0);

/// @brief Returns the angle of the vector from p0 to p1, relative to the
/// positive X-axis.
/// @param p0 the first point
/// @param p1 the second point
/// @return the angle of the vector from p0 to p1
NV_EXTERN double nv_angle2(const struct nv_point p0, const struct nv_point p1);

/// @brief Tests whether the angle between p0-p1-p2 is acute.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return true if the angle is acute
NV_EXTERN bool nv_acute(const struct nv_point p0, const struct nv_point p1,
                        const struct nv_point p2);

/// @brief Tests whether the angle between p0-p1-p2 is obtuse.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return true if the angle is obtuse
NV_EXTERN bool nv_obtuse(const struct nv_point p0, const struct nv_point p1,
                         const struct nv_point p2);

/// @brief Returns the unoriented smallest angle between two vectors.
/// @param tip1 the first tip point
/// @param tail the tail point
/// @param tip2 the second tip point
/// @return the unoriented smallest angle between two vectors
NV_EXTERN double nv_angle_between(const struct nv_point tip1,
                                  const struct nv_point tail,
                                  const struct nv_point tip2);

/// @brief Computes the interior angle between two segments of a ring.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return the interior angle between two segments of a ring
NV_EXTERN bool nv_interior_angle(const struct nv_point p0,
                                 const struct nv_point p1,
                                 const struct nv_point p2);

/// @brief The bisector segment of AB-CD is (point, projection of point by \a
/// angle)
/// @param A
/// @param B
/// @param C
/// @param D
/// @param p
/// @param angle
/// @return
NV_EXTERN void nv_angle_bisector(const struct nv_point A,
                                 const struct nv_point B,
                                 const struct nv_point C,
                                 const struct nv_point D, struct nv_point *p,
                                 double *angle);

/* --------------------------------- Segment -------------------------------- */

/// @brief Computes the distance from a point p to a line segment AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
NV_EXTERN double nv_dis_point_to_segment(const struct nv_point p,
                                         const struct nv_point A,
                                         const struct nv_point B);

/// @brief Computes the perpendicular distance from a point p to the (infinite)
/// line containing the points AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
NV_EXTERN double nv_dis_point_to_perpendicular(const struct nv_point p,
                                               const struct nv_point A,
                                               const struct nv_point B);

NV_EXTERN void
nv_segment_intersection(const struct nv_point p1, const struct nv_point p2,
                        const struct nv_point p3, const struct nv_point p4,
                        const struct nv_point *pin, bool *intersection);

/* ----------------------------------- Box ---------------------------------- */

/// A rectangle defined by a minimum and maximum coordinates.
struct nv_box {
    struct nv_point min;
    struct nv_point max;
};

/// @brief Check two box is intersects
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return true if the two Envelopes intersect
NV_EXTERN bool nv_box_intersects(const struct nv_box env1,
                                 const struct nv_box env2);

/// @brief Computes the intersection of two Envelopes
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return the intersection of the two Envelopes
NV_EXTERN struct nv_box nv_box_intersection(const struct nv_box env1,
                                            const struct nv_box env2);

/// @brief Enlarges the boundary of the Envelope so that it contains
/// @param env1 the original Envelope
/// @param env2 the Envelope to be contained
/// @return the enlarged Envelope
NV_EXTERN struct nv_box nv_box_union(const struct nv_box env1,
                                     const struct nv_box env2);

/// @brief Convert nv_box object to nv_geobject
/// @param e the nv_box object
/// @param gdim the geometry dimension
/// @return the nv_geobject object
NV_EXTERN struct nv_geobject *nv_stroke_box(struct nv_box e, int gdim);

/* ---------------------------- Ellipse & Cricle ---------------------------- */

/// nv_ ellipse is used to describe an ellipse or circle.
/// Before V1.0, it would be treated as a regular geometric shape and
/// temporarily not included in the unified management of the nv_geom model,
/// while providing relevant algorithms for circles or ellipses.
struct nv_ellipse {
    struct nv_point center;
    double major;
    double minor;
    double azimuth;
};

/// The eccentricity of the ellipse. - double
#define MG_ELLIPSE_PROP_VALUE_ECCENTRICITY 0
/// The area of the ellipse. - double
#define MG_ELLIPSE_PROP_VALUE_AREA         1
/// The perimeter of the ellipse. - double
#define MG_ELLIPSE_PROP_VALUE_PERIMETER    2
/// Two foci of the ellipse. The axes are oriented by the azimuth and are on the
/// semi-major axis. - nv_point[2]
#define MG_ELLIPSE_PROP_VALUE_FOCI         4
/// The distance between the center and each foci. - double
#define MG_ELLIPSE_PROP_FOCUS_DISTANCE     8

/// Two points form a circle, and the line segment between these two points is
/// the diameter of the circle
#define MG_CONSTRUCT_CIRCLE_2P             1
/// Three points form a circle, and these three points are on the circle
#define MG_CONSTRUCT_CIRCLE_3P             2
/// To construct a circle with three tangent lines, six points need to be passed
/// in. These six points form three straight lines, which can generate 0-2
/// circles. They are also the inscribed circles of a triangle
#define MG_CONSTRUCT_CIRCLE_ICT            3

/// @brief Construct circles according to different calculation methods.
///
/// Construct circles according to different calculation methods. \a t is
/// determined based on the \a MG_CONSTRUCT_CIRCLE series macros.
/// For MG_CONSTRUCT_CIRCLE_2P andMG_CONSTRUCT_CIRCLE_3P, a circle will
/// ultimately be generated.
/// When the value of \a n is MG_CONSTRUCT_CIRCLE_ICT, if \a n is -1, the
/// algorithm for generating multiple tangent circles will be executed. If \a n
/// is -2, only one circle will be generated. And after the algorithm is
/// executed, the number of circles generated is transmitted out. External CS
/// needs to create an array of sufficient size according to requirements to
/// receive the return value.
///
/// @param p points
/// @param t calculation method
/// @param es circles
/// @param n number of circles
/// @return
NV_EXTERN void nv_construct_circle(const struct nv_point *p, int t,
                                   struct nv_ellipse *es, int *n);

/// @brief query ellipse attribute
///
/// Calculate the ellipse attribute. \a flags are a combination of
/// \a MG_ELLIPSE_PROP macro series. When passing in \a values externally, the
/// data structure needs to be organized by oneself. The algorithm will write
/// the calculation result in sequence based on the bits of the flag.
///
/// @param ell ellipse
/// @param flags query flag
/// @param values query result
/// @return
NV_EXTERN void nv_ellipse_prop_value(const struct nv_ellipse ell, int flags,
                                     double *values);

/// @brief stroke ellipse to nv_geobject
/// @param e ellipse
/// @param param geometry dim and segment count
/// Use the highest bit of an integer to represent the geometric dimension, 1:
/// line, 2: area. When passing other values, use the default dimension of 1;
/// The remaining digits represent the interpolation number. When the input
/// interpolation number is less than 3, the default interpolation number of 36
/// will be used.
/// @example
/// param: 246 create a polygon, segment to 46 linesegments
/// param: 52: error code, use default value
///
/// @return nv_geobject
NV_EXTERN struct nv_geobject *nv_stroke_ellipse(struct nv_ellipse e,
                                                uint32_t param);

/* ----------------------------------- Arc ---------------------------------- */

/// Arc segment
struct nv_arc {
    struct nv_point start;
    struct nv_point along;
    struct nv_point end;
};

/// @brief stroke arc to nv_geobject
/// @param arc arc
/// @param maxAngleStepSizeDegress max angle step size
/// @return nv_geobject
NV_EXTERN struct nv_geobject *nv_stroke_arc(struct nv_arc arc,
                                            double maxAngleStepSizeDegress);

struct nv_geobject;

NV_EXTERN struct nv_geobject *nv_create_single(int gdim, int pn, int cdim,
                                               const double *pp, int flag);
NV_EXTERN struct nv_geobject *nv_create_multi(int gdim, int snum,
                                              struct nv_geobject **subs);

/// @brief free geometry object
/// @param obj
/// @return
NV_EXTERN void nv_free_object(struct nv_geobject *obj);

/// @brief return geometry coordinate dimension
/// @param obj
/// @return 2 or 3
NV_EXTERN int nv_dim_c(const struct nv_geobject *obj);

/// @brief return geometry dimension
/// @param obj
/// @return 0:point, 1:line, 2:area
/// TODO: 3:collection
NV_EXTERN int nv_dim_g(const struct nv_geobject *obj);

/// @brief return geometry number
/// @param obj
/// @return geometry number
NV_EXTERN int nv_sub_n(const struct nv_geobject *obj);

/// @brief return sub of multi geometry
/// @param obj multi geometry
/// @param i index
/// @return sub geometry, When \a obj is a single geometry object, returns
/// itself
NV_EXTERN struct nv_geobject *nv_sub_at(const struct nv_geobject *obj, int i);

/// @brief return point number
/// @param obj
/// @return point number
NV_EXTERN int nv_point_n(const struct nv_geobject *obj);

/* --------------------------------- Factory -------------------------------- */

enum {
    GEOMETRY_IO_WKT,     ///< geometry IO type wkt
    GEOMETRY_IO_WKB,     ///< geometry IO type wkb
    GEOMETRY_IO_WKB_HEX, ///< geometry IO type wkb hex
    GEOMETRY_IO_GEOJSON, ///< geometry IO type geojson
    GEOMETRY_IO_EWKT,    ///< geometry IO type ewkt
    GEOMETRY_IO_EWKB,    ///< geometry IO type ewkb
    GEOMETRY_IO_KML,     ///< geometry IO type kml
    GEOMETRY_IO_GML2,    ///< geometry IO type gml2
    GEOMETRY_IO_GML3     ///< geometry IO type gml3
};

NV_EXTERN struct nv_geobject *nv_read(int flag, const char *data, size_t len);

NV_EXTERN int nv_write(int flag, const struct nv_geobject *obj, char **data,
                       size_t *len);

struct sdo_geometry {
    int sdo_gtype;
    int sdo_srid;
    int sdo_elem_count;
    int *sdo_elem_info;
    int sdo_ord_count;
    double *sdo_ordinates;
};

NV_EXTERN struct nv_geobject *nv_read_ora(const struct sdo_geometry sdo,
                                          int flag);

NV_EXTERN int nv_write_ora(const struct nv_geobject *obj,
                           struct sdo_geometry *sdo);

/* ------------------------- geometry reader writer ------------------------- */

struct nv_i4;

struct nv_reader2;

NV_EXTERN struct nv_geobject *nv_i4_object(struct nv_i4 *i4);

NV_EXTERN void nv_i4_propProp(const struct nv_i4 *i4, int *propSize,
                              int **prop);

NV_EXTERN int nv_i4_prop_value(const struct nv_i4 *i4, size_t index);

NV_EXTERN struct nv_reader2 *nv_reader2_new(size_t size);

NV_EXTERN void nv_free_reader2(struct nv_reader2 *reader);

NV_EXTERN void nv_reader2_push(struct nv_reader2 *reader,
                               const struct nv_geobject *obj, int propSize,
                               int *prop);

NV_EXTERN struct nv_i4 *nv_reader2_iterator(struct nv_reader2 *writer);

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
NV_EXTERN double nv_tolerance(double tol);

/// Get the value property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_VALUE_*
NV_EXTERN double nv_prop_value(const struct nv_geobject *obj, int mode);

/// Get the geometric object property of a geometric object
/// Candidate values for mode are GEOMETRY_PROP_GEO_*, except
/// GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE and GEOMETRY_PROP_GEO_INNER_CIRCLE.
NV_EXTERN struct nv_geobject *nv_prop_geo(const struct nv_geobject *obj,
                                          int mode);

NV_EXTERN void nv_prop_geo2(const struct nv_geobject *obj, int mode,
                            double *paras);

/// @brief Check if the point is on the left or right side of the line
/// @param obj line object
/// @param xy point coordinates
/// @return 0: The distance from the point to the line is less than the
/// tolerance; 1: left; 2: right
NV_EXTERN int nv_left_right(const struct nv_geobject *obj, double *xy);

/// @brief Computes whether a ring defined by a geom::CoordinateSequence is
/// oriented counter-clockwise.
/// @param pp point pointer
/// @param npoints point number
/// @param cdim coordinate dim
NV_EXTERN bool nv_ccw(const double *pp, int npoints, int cdim);

/// @brief Get the convexity of a vertex on a ring object
/// @param obj single ring object
/// @param index vertex index When index is -1, get the concave and convex
/// properties of all vertices
/// @param convex convexity of the vertex
NV_EXTERN void nv_vertex_convex(const struct nv_geobject *obj, int index,
                                int *convex);

/// @brief Make the rings formed by the building outlines extracted from the
/// city image rectangular
/// @param xy building outline coordinates, Points may be added or deleted
/// within the algorithm
/// @param np number of points
NV_EXTERN void nv_building_regularization(double *xy, int np);

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
NV_EXTERN void nv_kmeans(struct nv_reader2 *P, int n, struct nv_reader2 **W);

// rtree_new returns a new rtree
//
// Returns NULL if the system is out of memory.
NV_EXTERN struct rtree *rtree_new(void);

// rtree_free frees an rtree
NV_EXTERN void rtree_free(struct rtree *tr);

// rtree_clone makes an instant copy of the btree.
//
// This operation uses shadowing / copy-on-write.
NV_EXTERN struct rtree *rtree_clone(struct rtree *tr);

// rtree_set_item_callbacks sets the item clone and free callbacks that will be
// called internally by the rtree when items are inserted and removed.
//
// These callbacks are optional but may be needed by programs that require
// copy-on-write support by using the rtree_clone function.
//
// The clone function should return true if the clone succeeded or false if the
// system is out of memory.
NV_EXTERN void rtree_set_item_callbacks(
    struct rtree *tr, bool (*clone)(const void *item, void **into, void *udata),
    void (*free)(const void *item, void *udata));

// rtree_set_udata sets the user-defined data.
//
// This should be called once after rtree_new() and is only used for
// the item callbacks as defined in rtree_set_item_callbacks().
NV_EXTERN void rtree_set_udata(struct rtree *tr, void *udata);

// rtree_insert inserts an item into the rtree.
//
// This operation performs a copy of the data that is pointed to in the second
// and third arguments. The R-tree expects a rectangle, which is two arrays of
// doubles. The first N values as the minimum corner of the rect, and the next
// N values as the maximum corner of the rect, where N is the number of
// dimensions.
//
// When inserting points, the max coordinates is optional (set to NULL).
//
// Returns false if the system is out of memory.
NV_EXTERN bool rtree_insert(struct rtree *tr, const double *min,
                            const double *max, const void *data);

// rtree_search searches the rtree and iterates over each item that intersect
// the provided rectangle.
//
// Returning false from the iter will stop the search.
NV_EXTERN void rtree_search(const struct rtree *tr, const double *min,
                            const double *max,
                            bool (*iter)(const double *min, const double *max,
                                         const void *data, void *udata),
                            void *udata);

// rtree_scan iterates over every item in the rtree.
//
// Returning false from the iter will stop the scan.
NV_EXTERN void rtree_scan(const struct rtree *tr,
                          bool (*iter)(const double *min, const double *max,
                                       const void *data, void *udata),
                          void *udata);

// rtree_count returns the number of items in the rtree.
NV_EXTERN size_t rtree_count(const struct rtree *tr);

// rtree_delete deletes an item from the rtree.
//
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a binary comparison of its data to the provided
// data. The first item that is found is deleted.
//
// Returns false if the system is out of memory.
NV_EXTERN bool rtree_delete(struct rtree *tr, const double *min,
                            const double *max, const void *data);

// rtree_delete_with_comparator deletes an item from the rtree.
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a comparison of its data to the provided data using
// a compare function. The first item that is found is deleted.
//
// Returns false if the system is out of memory.
NV_EXTERN bool rtree_delete_with_comparator(
    struct rtree *tr, const double *min, const double *max, const void *data,
    int (*compare)(const void *a, const void *b, void *udata), void *udata);

// rtree_opt_relaxed_atomics activates memory_order_relaxed for all atomic
// loads. This may increase performance for single-threaded programs.
// Optionally, define RTREE_NOATOMICS to disbale all atomics.
NV_EXTERN void rtree_opt_relaxed_atomics(struct rtree *tr);

// #define NMEA_NV_MAXSAT    (12)
// #define NMEA_SATINPACK (4)
// #define NMEA_NSATPACKS (NMEA_NV_MAXSAT / NMEA_SATINPACK)

// /**
//  * Information about satellite
//  * \see nmeaSATINFO
//  * \see nmeaGPGSV
//  */
// struct nmeaSATELLITE {
//     int id;      ///< Satellite PRN number
//     int in_use;  ///< Used in position fix
//     int elv;     ///< Elevation in degrees, 90 maximum
//     int azimuth; ///< Azimuth, degrees from true north, 000 to 359
//     int sig;     ///< Signal, 00-99 dB
// };

// struct nmeaTIME {
//     int year; ///< Years since 1900
//     int mon;  ///< Months since January - [0,11]
//     int day;  ///< Day of the month - [1,31]
//     int hour; ///< Hours since midnight - [0,23]
//     int min;  ///< Minutes after the hour - [0,59]
//     int sec;  ///< Seconds after the minute - [0,59]
//     int msec; ///< Thousandths part of second - [0,999]
// };

// /**
//  * \brief Get time now to nmeaTIME structure
//  */
// void nmea_time_now(struct nmeaTIME *t);

// /**
//  * NMEA packets type which parsed and generated by library
//  */
// enum nmeaPACKTYPE {
//     GPNON = 0x0000, ///< Unknown packet type.
//     GPGGA = 0x0001, ///< GGA - Essential fix data which provide 3D location
//     and
//                     ///< accuracy data.
//     GPGSA = 0x0002, ///< GSA - GPS receiver operating mode, SVs used for
//                     ///< navigation, and DOP values.
//     GPGSV = 0x0004, ///< GSV - Number of SVs in view, PRN numbers, elevation,
//                     ///< azimuth & SNR values.
//     GPRMC = 0x0008, ///< RMC - Recommended Minimum Specific GPS/TRANSIT Data.
//     GPVTG = 0x0010, ///< VTG - Actual track made good and speed over ground.
//     GPGST = 0x0012, ///< GST - GPS Pseudorange Noise Statistics
//     HCHDG = 0x0020, ///< HDG - Heading, Deviation and Variation
//     HCHDT = 0x0100, ///< HDT - Heading reference to true north
// };

// /**
//  * GGA packet information structure (Global Positioning System Fix Data)
//  */
// struct nmeaGPGGA {
//     struct nmeaTIME utc; ///< UTC of position (just time)
//     double lat;          ///< Latitude in NDEG - [degree][min].[sec/60]
//     char ns;             ///< [N]orth or [S]outh
//     double lon;          ///< Longitude in NDEG - [degree][min].[sec/60]
//     char ew;             ///< [E]ast or [W]est
//     int sig; ///< GPS quality indicator (0 = Invalid; 1 = Fix; 2 =
//     Differential,
//              ///< 3 = Sensitive)
//     int satinuse;   ///< Number of satellites in use (not those in view)
//     double HDOP;    ///< Horizontal dilution of precision
//     double elv;     ///< Antenna altitude above/below mean sea level (geoid)
//     char elv_units; ///< [M]eters (Antenna height unit)
//     double diff; ///< Geoidal separation (Diff. between WGS-84 earth
//     ellipsoid
//                  ///< and mean sea level. '-' = geoid is below WGS-84
//                  ellipsoid)
//     char diff_units; ///< [M]eters (Units of geoidal separation)
//     double dgps_age; ///< Time in seconds since last DGPS update
//     int dgps_sid;    ///< DGPS station ID number
// };

// /**
//  * GST packet information structure (GPS Pseudorange Noise Statistics)
//  */
// struct nmeaGPGST {
//     struct nmeaTIME utc; ///< UTC of position fix
//     double rms_pr; ///< RMS value of the pseudorange residuals; Includes
//     carrier
//                    ///< phase residuals during periods of RTK (float) and RTK
//                    ///< (fixed) processing
//     double
//         err_major; ///< Error ellipse semi-major axis 1 sigma error, in
//         meters
//     double
//         err_minor;  ///< Error ellipse semi-minor axis 1 sigma error, in
//         meters
//     double err_ori; ///< Error ellipse orientation, degrees from true north
//     double sig_lat; ///< Latitude 1 sigma error, in meters
//     double sig_lon; ///< Longitude 1 sigma error, in meters
//     double sig_alt; ///< Height 1 sigma error, in meters
// };

// /**
//  * GSA packet information structure (Satellite status)
//  */
// struct nmeaGPGSA {
//     char fix_mode; ///< Mode (M = Manual, forced to operate in 2D or 3D; A =
//                    ///< Automatic, 3D/2D)
//     int fix_type; ///< Type, used for navigation (1 = Fix not available; 2 =
//     2D;
//                   ///< 3 = 3D)
//     int sat_prn[NMEA_NV_MAXSAT]; ///< PRNs of satellites used in position fix
//     (null
//                               ///< for unused fields)
//     double PDOP;              ///< Dilution of precision
//     double HDOP;              ///< Horizontal dilution of precision
//     double VDOP;              ///< Vertical dilution of precision
//     char pack_type;           ///< P=GPS, N=generic, L=GLONASS

// } nmeaGPGSA;

// /**
//  * GSV packet information structure (Satellites in view)
//  */
// struct nmeaGPGSV {
//     int pack_count; ///< Total number of messages of this type in this cycle
//     int pack_index; ///< Message number
//     int sat_count;  ///< Total number of satellites in view
//     char pack_type; ///< P=GPS - S=SBas - N=generic - L=GLONAS - A=GALILEO -
//                     ///< B=BEIDOU - Q=QZSS
//     struct nmeaSATELLITE sat_data[NMEA_SATINPACK];
// };

// /**
//  * RMC packet information structure (Recommended Minimum sentence C)
//  */
// struct nmeaGPRMC {
//     struct nmeaTIME utc; ///< UTC of position
//     char status;         ///< Status (A = active or V = void)
//     double lat;          ///< Latitude in NDEG - [degree][min].[sec/60]
//     char ns;             ///< [N]orth or [S]outh
//     double lon;          ///< Longitude in NDEG - [degree][min].[sec/60]
//     char ew;             ///< [E]ast or [W]est
//     double speed;        ///< Speed over the ground in knots
//     double direction;    ///< Track angle in degrees True
//     double declination; ///< Magnetic variation degrees (Easterly var.
//     subtracts
//                         ///< from true course)
//     char declin_ew;     ///< [E]ast or [W]est
//     char mode;          ///< Mode indicator of fix type (A = autonomous, D =
//                ///< differential, E = estimated, N = not valid, S =
//                simulator)
//     char navstatus; ///< NMEA v4.1 - Navigation Status type (S = Safe, C =
//                     ///< Caution, U = Unsafe, V = Navigational status not
//                     valid)
// };

// /**
//  * VTG packet information structure (Track made good and ground speed)
//  */
// struct nmeaGPVTG {
//     double dir; ///< True track made good (degrees)
//     char dir_t; ///< Fixed text 'T' indicates that track made good is
//     relative
//                 ///< to true north
//     double dec; ///< Magnetic track made good
//     char dec_m; ///< Fixed text 'M'
//     double spn; ///< Ground speed, knots
//     char spn_n; ///< Fixed text 'N' indicates that speed over ground is in
//     knots double spk; ///< Ground speed, kilometers per hour char spk_k; ///<
//     Fixed text 'K' indicates that speed over ground is in
//                 ///< kilometers/hour
// };

// /**
//  * HDT packet information structure (Heading from True North)
//  */
// struct nmeaGPHDT {
//     double heading; ///< Heading in degrees
//     char t_flag; ///< Fixed text 'T' indicates that heading is relative to
//     true
//                  ///< north
// };

// /**
//  * HCHDG packet information structure (magnetic heading)
//  */
// struct nmeaHCHDG {
//     double mag_heading;   ///< Magnetic sensor heading (degrees)
//     double mag_deviation; ///< Magnetic deviation (degrees)
//     char ew_deviation;    ///< [E]ast or [W]est
//     double mag_variation; ///< Magnetic variation (degrees)
//     char ew_variation;    ///< [E]ast or [W]est
// };

// /**
//  * HDT packet information structure (Heading, )
//  */
// struct nmeaHCHDT {
//     double direction; ///< Heading respect to true north (degrees)
//     char t_flag;      ///< Static text [T]
// };

// NV_EXTERN void nmea_zero_GPGGA(struct nmeaGPGGA *pack);
// NV_EXTERN void nmea_zero_GPGST(struct nmeaGPGST *pack);
// NV_EXTERN void nmea_zero_GPGSA(struct nmeaGPGSA *pack);
// NV_EXTERN void nmea_zero_GPGSV(struct nmeaGPGSV *pack);
// NV_EXTERN void nmea_zero_GPRMC(struct nmeaGPRMC *pack);
// NV_EXTERN void nmea_zero_GPVTG(struct nmeaGPVTG *pack);

// /**
//  * Position data in fractional degrees or radians
//  */
// struct nmeaPOS {
//     double lat; ///< Latitude
//     double lon; ///< Longitude
// };

// /**
//  * Information about all satellites in view
//  * \see nmeaINFO
//  * \see nmeaGPGSV
//  */
// struct nmeaSATINFO {
//     int inuse;  ///< Number of satellites in use (not those in view)
//     int inview; ///< Total number of satellites in view
//     struct nmeaSATELLITE sat[NMEA_NV_MAXSAT]; ///< Satellites information
// };

// /**
//  * Summary GPS information from all parsed packets,
//  * used also for generating NMEA stream
//  * \see nmea_parse
//  * \see nmea_GPGGA2info, nmea_...2info
//  */
// struct nmeaINFO {
//     int smask; ///< Mask specifying types of packages from which data have
//     been
//                ///< obtained

//     struct nmeaTIME utc; ///< UTC of position

//     int sig; ///< GPS quality indicator (0 = Invalid; 1 = Fix; 2 =
//     Differential,
//              ///< 3 = Sensitive)
//     int fix; ///< Operating mode, used for navigation (1 = Fix not available;
//     2
//              ///< = 2D; 3 = 3D)

//     double PDOP; ///< Position Dilution Of Precision
//     double HDOP; ///< Horizontal Dilution Of Precision
//     double VDOP; ///< Vertical Dilution Of Precision

//     double lat; ///< Latitude in NDEG - +/-[degree][min].[sec/60]
//     double lon; ///< Longitude in NDEG - +/-[degree][min].[sec/60]
//     double
//         elv; ///< Antenna altitude above/below mean sea level (geoid) in
//         meters
//     double speed;       ///< Speed over the ground in kilometers/hour
//     double direction;   ///< Track angle in degrees True
//     double declination; ///< Magnetic variation degrees (Easterly var.
//     subtracts
//                         ///< from true course)
//     double rms_pr; ///< RMS value of the pseudorange residuals; includes
//     carrier
//                    ///< phase residuals during periods of RTK (float) and RTK
//                    ///< (fixed) processing
//     double
//         err_major; ///< Error ellipse semi-major axis 1 sigma error, in
//         meters
//     double
//         err_minor;  ///< Error ellipse semi-minor axis 1 sigma error, in
//         meters
//     double err_ori; ///< Error ellipse orientation, degrees from true north
//     double sig_lat; ///< Latitude 1 sigma error, in meters
//     double sig_lon; ///< Longitude 1 sigma error, in meters
//     double sig_alt; ///< Height 1 sigma error, in meters

//     struct nmeaSATINFO satinfo; ///< Satellites information
// };

// void nmea_zero_INFO(struct nmeaINFO *info);

// NV_EXTERN int nmea_pack_type(const char *buff, int buff_sz);
// NV_EXTERN int nmea_find_tail(const char *buff, int buff_sz, int *res_crc);

// NV_EXTERN int nmea_parse_GPGGA(const char *buff, int buff_sz,
//                                struct nmeaGPGGA *pack);
// NV_EXTERN int nmea_parse_GPGSA(const char *buff, int buff_sz,
//                                struct nmeaGPGSA *pack);
// NV_EXTERN int nmea_parse_GPGSV(const char *buff, int buff_sz,
//                                struct nmeaGPGSV *pack);
// NV_EXTERN int nmea_parse_GPRMC(const char *buff, int buff_sz,
//                                struct nmeaGPRMC *pack);
// NV_EXTERN int nmea_parse_GPVTG(const char *buff, int buff_sz,
//                                struct nmeaGPVTG *pack);
// NV_EXTERN int nmea_parse_HCHDG(const char *buff, int buff_sz,
//                                struct nmeaHCHDG *pack);
// NV_EXTERN int nmea_parse_HCHDT(const char *buff, int buff_sz,
//                                struct nmeaHCHDT *pack);
// NV_EXTERN int nmea_parse_GPGST(const char *buff, int buff_sz,
//                                struct nmeaGPGST *pack);
// NV_EXTERN int nmea_parse_GPHDT(const char *buff, int buff_sz,
//                                struct nmeaGPHDT *pack);

// NV_EXTERN void nmea_GPGGA2info(struct nmeaGPGGA *pack, struct nmeaINFO
// *info); NV_EXTERN void nmea_GPGST2info(struct nmeaGPGST *pack, struct
// nmeaINFO *info); NV_EXTERN void nmea_GPGSA2info(struct nmeaGPGSA *pack,
// struct nmeaINFO *info); NV_EXTERN void nmea_GPGSV2info(struct nmeaGPGSV
// *pack, struct nmeaINFO *info); NV_EXTERN void nmea_GPRMC2info(struct
// nmeaGPRMC *pack, struct nmeaINFO *info); NV_EXTERN void
// nmea_GPVTG2info(struct nmeaGPVTG *pack, struct nmeaINFO *info);

// /*
//  * degree VS radian
//  */
// NV_EXTERN double nmea_degree2radian(double val);
// NV_EXTERN double nmea_radian2degree(double val);

// /*
//  * NDEG (NMEA degree)
//  */
// NV_EXTERN double nmea_ndeg2degree(double val);
// NV_EXTERN double nmea_degree2ndeg(double val);

// NV_EXTERN double nmea_ndeg2radian(double val);
// NV_EXTERN double nmea_radian2ndeg(double val);

// /*
//  * DOP
//  */
// NV_EXTERN double nmea_calc_pdop(double hdop, double vdop);
// NV_EXTERN double nmea_dop2meters(double dop);
// NV_EXTERN double nmea_meters2dop(double meters);

// /*
//  * positions work
//  */
// NV_EXTERN void nmea_info2pos(const struct nmeaINFO *info, struct nmeaPOS
// *pos); NV_EXTERN void nmea_pos2info(const struct nmeaPOS *pos, struct
// nmeaINFO *info);

// NV_EXTERN double nmea_distance(const struct nmeaPOS *from_pos,
//                                const struct nmeaPOS *to_pos);

// NV_EXTERN double nmea_distance_ellipsoid(const struct nmeaPOS *from_pos,
//                                          const struct nmeaPOS *to_pos,
//                                          double *from_azimuth,
//                                          double *to_azimuth);

// NV_EXTERN int nmea_move_horz(const struct nmeaPOS *start_pos,
//                              struct nmeaPOS *end_pos, double azimuth,
//                              double distance);

// NV_EXTERN int nmea_move_horz_ellipsoid(const struct nmeaPOS *start_pos,
//                                        struct nmeaPOS *end_pos, double
//                                        azimuth, double distance, double
//                                        *end_azimuth);

/* Direction north */
#define GEOHASH_DIRECTION_NORTH      0
/* Direction east */
#define GEOHASH_DIRECTION_EAST       1
/* Direction west */
#define GEOHASH_DIRECTION_WEST       2
/* Direction south */
#define GEOHASH_DIRECTION_SOUTH      3
/* Direction south west */
#define GEOHASH_DIRECTION_SOUTH_WEST 4
/* Direction south east */
#define GEOHASH_DIRECTION_SOUTH_EAST 5
/* Direction north west */
#define GEOHASH_DIRECTION_NORTH_WEST 6
/* Direction north east */
#define GEOHASH_DIRECTION_NORTH_EAST 7

typedef struct geohash_bits {
    uint64_t bits;
    uint8_t step;
} geohash_bits;

typedef struct geohash_neighbors {
    geohash_bits north;
    geohash_bits east;
    geohash_bits west;
    geohash_bits south;
    geohash_bits south_west;
    geohash_bits south_east;
    geohash_bits north_west;
    geohash_bits north_east;
} geohash_neighbors;

typedef struct geohash_range {
    double max;
    double min;
} geohash_range;

typedef struct geohash_area {
    geohash_bits hash;
    geohash_range longitude;
    geohash_range latitude;
} geohash_area;

NV_EXTERN void get_geohash_range(geohash_range *long_range,
                                 geohash_range *lat_range);
NV_EXTERN int geohash_encode(const geohash_range *long_range,
                             const geohash_range *lat_range, double longitude,
                             double latitude, uint8_t step, geohash_bits *hash);
NV_EXTERN int geohash_encode_type(double longitude, double latitude,
                                  uint8_t step, geohash_bits *hash);
NV_EXTERN int geohash_encode_WGS84(double longitude, double latitude,
                                   uint8_t step, geohash_bits *hash);

NV_EXTERN int geohash_decode(const geohash_range long_range,
                             const geohash_range lat_range,
                             const geohash_bits hash, geohash_area *area);
NV_EXTERN int geohash_decode_type(const geohash_bits hash, geohash_area *area);
NV_EXTERN int geohash_decode_WGS84(const geohash_bits hash, geohash_area *area);
NV_EXTERN int geohash_decode_area_longlat(const geohash_area *area, double *xy);
NV_EXTERN int geohash_decode_longlat_type(const geohash_bits hash, double *xy);
NV_EXTERN int geohash_decode_longlat_WGS84(const geohash_bits hash, double *xy);

NV_EXTERN void geohash_query_neighbors(const geohash_bits *hash,
                                       geohash_neighbors *neighbors);

struct mm_texture {
    char *name;
    char *path;
};

struct mm_material {
    /* Material name */
    char *name;

    /* Parameters */
    float Ka[3]; /* Ambient */
    float Kd[3]; /* Diffuse */
    float Ks[3]; /* Specular */
    float Ke[3]; /* Emission */
    float Kt[3]; /* Transmittance */
    float Ns;    /* Shininess */
    float Ni;    /* Index of refraction */
    float Tf[3]; /* Transmission filter */
    float d;     /* Disolve (alpha) */
    int illum;   /* Illumination model */

    /* Set for materials that don't come from the associated mtllib */
    int fallback;

    /* Texture map indices in fastObjMesh textures array */
    unsigned int map_Ka;
    unsigned int map_Kd;
    unsigned int map_Ks;
    unsigned int map_Ke;
    unsigned int map_Kt;
    unsigned int map_Ns;
    unsigned int map_Ni;
    unsigned int map_d;
    unsigned int map_bump;
};

#ifdef __cpluscplus
}
#endif

#endif