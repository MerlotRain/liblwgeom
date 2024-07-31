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

/// PointXY
/// The base point type used for all geometries.
struct mg_point {
    double x;
    double y;
};

/// Returns a new point which corresponds to this point projected by a specified
/// distance with specified angles
EXTERN struct mg_point mg_point_project(const struct mg_point p, double dis,
                                        double azimuth);

/// point angle with mg_point(0, 0)
EXTERN double mg_angle(const struct mg_point p0);

/// Returns the angle of the vector from p0 to p1, relative to the positive
/// X-axis.
EXTERN double mg_angle2(const struct mg_point p0, const struct mg_point p1);

/// Tests whether the angle between p0-p1-p2 is acute.
EXTERN bool mg_acute(const struct mg_point p0, const struct mg_point p1,
                     const struct mg_point p2);

/// Tests whether the angle between p0-p1-p2 is obtuse.
EXTERN bool mg_obtuse(const struct mg_point p0, const struct mg_point p1,
                      const struct mg_point p2);

/// Returns the unoriented smallest angle between two vectors.
EXTERN double mg_angle_between(const struct mg_point tip1,
                               const struct mg_point tail,
                               const struct mg_point tip2);

/// Computes the interior angle between two segments of a ring.
EXTERN bool mg_interior_angle(const struct mg_point p0,
                              const struct mg_point p1,
                              const struct mg_point p2);

/// Envelope
/// A rectangle defined by a minimum and maximum coordinates.
struct mg_envelope {
    struct mg_point min;
    struct mg_point max;
};

/// Computes the intersection of two Envelopes
EXTERN struct mg_envelope mg_env_intersection(const struct mg_envelope env1,
                                              const struct mg_envelope env2);

/// Enlarges the boundary of the Envelope so that it contains
EXTERN struct mg_envelope mg_env_union(const struct mg_envelope env1,
                                       const struct mg_envelope env2);

/// Tests if the Envelope `other` lies wholly inside this Envelope
/// (inclusive of the boundary).
EXTERN bool mg_env_contains(const struct mg_envelope env1,
                            const struct mg_envelope env2);

/// Returns `true` if the given point lies in or on the envelope.
EXTERN bool mg_env_contains_point(const struct mg_envelope env, double *xy);

/// mg_ ellipse is used to describe an ellipse or circle.
/// Before V1.0, it would be treated as a regular geometric shape and
/// temporarily not included in the unified management of the mg_geom model,
/// while providing relevant algorithms for circles or ellipses.
struct mg_ellipse {
    struct mg_point center;
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
/// semi-major axis. - mg_point[2]
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

/// Calculate the ellipse attribute. \a flags are a combination of
/// \a MG_ELLIPSE_PROP macro series. When passing in \a values externally, the
/// data structure needs to be organized by oneself. The algorithm will write
/// the calculation result in sequence based on the bits of the flag.
EXTERN void mg_ellipse_prop_value(const struct mg_ellipse ell, int flags,
                                  double *values);

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
EXTERN void mg_construct_circle(const struct mg_point *p, int t,
                                struct mg_ellipse *es, int *n);

/// Mini Geometry
struct mg_path;
struct mg_ring;
struct mg_polygon;
struct mg_geom;

/// Geometry types.
enum geom_type {
    MG_POINT = 1,              ///< Point
    MG_PATH = 2,               ///< Path
    MG_RING = 3,               ///< Ring, closed path
    MG_POLYGON = 4,            ///< Polygon
    MG_MULTIPOINT = 5,         ///< MultiPoint, collection of points
    MG_MULTILINESTRING = 6,    ///< MultiLinestring, collection of paths
    MG_MULTIPOLYGON = 7,       ///< MultiPolygon, collection of polygons
    MG_GEOMETRYCOLLECTION = 8, ///< GeometryCollection, collection of geometries
};

#define MG_COORDINATE_FLAG_XY 1 << 0 ///< base coordinate flag
#define MG_COORDINATE_FLAG_Z  1 << 1 ///< coordinate ptr order by x, y, z
#define MG_COORDINATE_FLAG_M  1 << 2 ///< coordinate ptr order by x, y, m

/// @brief create point geometry
/// @param flag Extract and combine values from MG-COORDINATe_LAG series macros
/// @param pp point coordinate ptr
EXTERN struct mg_geom *geom_new_point(int flag, const double *pp);

/// @brief create path geometry
/// @param flag Extract and combine values from MG-COORDINATe_LAG series macros
/// @param pp path coordinate ptr
/// @param np number of point
EXTERN struct mg_geom *geom_new_path(int flag, const double *pp, int np);

/// @brief create path geometry
/// @param flag Extract and combine values from MG-COORDINATe_LAG series macros
/// @param pp ring coordinate ptr
/// @param np number of point
EXTERN struct mg_geom *geom_new_ring(int flag, const double *rp, int np);

/// @brief create path geometry
/// @param flag Extract and combine values from MG-COORDINATe_LAG series macros
/// @param sp polygon shell ring ptr
/// @param spn shell ring point number
/// @param hpp polygon holes pointer
/// @param hpn the array of single hole point number
/// @param hppn hpn number
EXTERN struct mg_geom *geom_new_polygon(int flag, const double *sp, int spn,
                                        const double **hpp, int *hpn, int hppn);

/// @brief create multi geometry
/// @param gt geometry type
/// @param geoms single geometry struct pointer
/// @param ng number of geometry
EXTERN struct mg_geom *geom_new_multigeom(int gt, const struct mg_geom **geoms,
                                          int ng);

/// @brief free geometry
/// @param geom
EXTERN void geom_free(struct mg_geom *geom);

EXTERN struct mg_geom *geom_read_wkt(const char *data, int len);
EXTERN struct mg_geom *geom_read_wkb(const char *data, int len);
EXTERN struct mg_geom *geom_read_geojson(const char *data, int len);
EXTERN struct mg_geom *geom_read_ewkt(const char *data, int len);
EXTERN struct mg_geom *geom_read_ewkb(const char *data, int len);
EXTERN struct mg_geom *geom_read_kml(const char *data, int len);
EXTERN struct mg_geom *geom_read_gml(const char *data, int len);
EXTERN struct mg_geom *geom_read_gml2(const char *data, int len);

EXTERN int geom_write_wkt(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_wkb(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_geojson(const struct mg_geom *g, char *data, int len);
EXTERN int geom_write_ewkt(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_ewkb(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_kml(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_gml(const struct mg_geom *g, char **data, int len);
EXTERN int geom_write_gml2(const struct mg_geom *g, char **data, int len);

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

EXTERN double geom_prop_value(const struct mg_geom *geom, int mode);

EXTERN struct mg_geom *geom_prop_geo(const struct mg_geom *geom, int mode);

EXTERN void geom_prop_geo2(const struct mg_geom *geom, int mode, double *paras);

#ifdef __cplusplus
}
#endif

#endif