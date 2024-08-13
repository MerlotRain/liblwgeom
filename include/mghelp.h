#ifndef MGHELP_H
#define MGHELP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mathse.h>

/* ---------------------------------- Point --------------------------------- */

/// The base point type used for all geometries.
struct mg_point {
    double x;
    double y;
};

/// @brief point angle with mg_point(0, 0)
/// @param p the point to compute the angle for
/// @return the angle of the point
EXTERN double mg_angle(const struct mg_point p0);

/// @brief Returns the angle of the vector from p0 to p1, relative to the
/// positive X-axis.
/// @param p0 the first point
/// @param p1 the second point
/// @return the angle of the vector from p0 to p1
EXTERN double mg_angle2(const struct mg_point p0, const struct mg_point p1);

/// @brief Tests whether the angle between p0-p1-p2 is acute.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return true if the angle is acute
EXTERN bool mg_acute(const struct mg_point p0, const struct mg_point p1,
                     const struct mg_point p2);

/// @brief Tests whether the angle between p0-p1-p2 is obtuse.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return true if the angle is obtuse
EXTERN bool mg_obtuse(const struct mg_point p0, const struct mg_point p1,
                      const struct mg_point p2);

/// @brief Returns the unoriented smallest angle between two vectors.
/// @param tip1 the first tip point
/// @param tail the tail point
/// @param tip2 the second tip point
/// @return the unoriented smallest angle between two vectors
EXTERN double mg_angle_between(const struct mg_point tip1,
                               const struct mg_point tail,
                               const struct mg_point tip2);

/// @brief Computes the interior angle between two segments of a ring.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return the interior angle between two segments of a ring
EXTERN bool mg_interior_angle(const struct mg_point p0,
                              const struct mg_point p1,
                              const struct mg_point p2);

/// @brief The bisector segment of AB-CD is (point, projection of point by \a
/// angle)
/// @param A
/// @param B
/// @param C
/// @param D
/// @param p
/// @param angle
/// @return
EXTERN void mg_angle_bisector(const struct mg_point A, const struct mg_point B,
                              const struct mg_point C, const struct mg_point D,
                              struct mg_point *p, double *angle);

/* --------------------------------- Segment -------------------------------- */

/// @brief Computes the distance from a point p to a line segment AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
EXTERN double mg_dis_point_to_segment(const struct mg_point p,
                                      const struct mg_point A,
                                      const struct mg_point B);

/// @brief Computes the perpendicular distance from a point p to the (infinite)
/// line containing the points AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
EXTERN double mg_dis_point_to_perpendicular(const struct mg_point p,
                                            const struct mg_point A,
                                            const struct mg_point B);

EXTERN void
mg_segment_intersection(const struct mg_point p1, const struct mg_point p2,
                        const struct mg_point p3, const struct mg_point p4,
                        const struct mg_point *pin, bool *intersection);

/* ----------------------------------- Box ---------------------------------- */

/// A rectangle defined by a minimum and maximum coordinates.
struct mg_box {
    struct mg_point min;
    struct mg_point max;
};

/// @brief Check two box is intersects
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return true if the two Envelopes intersect
EXTERN bool mg_box_intersects(const struct mg_box env1,
                              const struct mg_box env2);

/// @brief Computes the intersection of two Envelopes
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return the intersection of the two Envelopes
EXTERN struct mg_box mg_box_intersection(const struct mg_box env1,
                                         const struct mg_box env2);

/// @brief Enlarges the boundary of the Envelope so that it contains
/// @param env1 the original Envelope
/// @param env2 the Envelope to be contained
/// @return the enlarged Envelope
EXTERN struct mg_box mg_box_union(const struct mg_box env1,
                                  const struct mg_box env2);

/// @brief Convert mg_box object to mg_object
/// @param e the mg_box object
/// @param gdim the geometry dimension
/// @return the mg_object object
EXTERN struct mg_object *mg_stroke_box(struct mg_box e, int gdim);

/* ---------------------------- Ellipse & Cricle ---------------------------- */

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
EXTERN void mg_construct_circle(const struct mg_point *p, int t,
                                struct mg_ellipse *es, int *n);

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
EXTERN void mg_ellipse_prop_value(const struct mg_ellipse ell, int flags,
                                  double *values);

/// @brief stroke ellipse to mg_object
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
/// @return mg_object
EXTERN struct mg_object *mg_stroke_ellipse(struct mg_ellipse e, uint32_t param);

/* ----------------------------------- Arc ---------------------------------- */

/// Arc segment
struct mg_arc {
    struct mg_point start;
    struct mg_point along;
    struct mg_point end;
};

/// @brief stroke arc to mg_object
/// @param arc arc
/// @param maxAngleStepSizeDegress max angle step size
/// @return mg_object
EXTERN struct mg_object *mg_stroke_arc(struct mg_arc arc,
                                       double maxAngleStepSizeDegress);

#ifdef __cplusplus
}
#endif

#endif