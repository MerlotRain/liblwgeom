#ifndef MGHELP_H
#define MGHELP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mg.h"
/// Point
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

/// Box
/// A rectangle defined by a minimum and maximum coordinates.
struct mg_box {
    struct mg_point min;
    struct mg_point max;
};

/// Computes the intersection of two Envelopes
EXTERN struct mg_box mg_box_intersection(const struct mg_box env1,
                                         const struct mg_box env2);

/// Enlarges the boundary of the Envelope so that it contains
EXTERN struct mg_box mg_box_union(const struct mg_box env1,
                                  const struct mg_box env2);

/// Tests if the Envelope `other` lies wholly inside this Envelope
/// (inclusive of the boundary).
EXTERN bool mg_box_contains(const struct mg_box env1, const struct mg_box env2);

/// Returns `true` if the given point lies in or on the envelope.
EXTERN bool mg_box_contains_point(const struct mg_box env, double *xy);

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

/// Convert mg_box object to mg_object
EXTERN struct mg_object *mg_trans_box(struct mg_box *e, int gdim);

EXTERN struct mg_object *mg_trans_ellipse(struct mg_ellipse *e, int gdim);

#ifdef __cplusplus
}
#endif

#endif