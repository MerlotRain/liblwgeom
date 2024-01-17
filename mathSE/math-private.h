#ifndef __MATH_PRIVATE_H__
#define __MATH_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Converts from radians to degrees. */
double math_to_degrees(double radians);

/* Converts from degrees to radians. */
double math_radians(double degrees);

/* Return the angle of the vector from (x1, y1) to (x2, y2). relative to the
 * positive x-axis. */
double math_angle(double x1, double y1, double x2, double y2);

/* Tests whether the angle between (x1, y1), (x2, y2), and (x3, y3) is acute. */
int math_is_acute(double x1, double y1, double x2, double y2, double x3,
                  double y3);

/* Returns the distance between (x1, y1) and (x2, y2). */
double math_distance(double x1, double y1, double x2, double y2);

double math_ccw_angle(double dy, double dx);

double math_sweep_angle(double cx, double cy, double x1, double y1, double x2,
                        double y2, double x3, double y3);

void math_circle_center_radius(double x1, double y1, double x2, double y2,
                               double x3, double y3, double *radius, double *cw,
                               double *cy);

double math_circle_length(double x1, double y1, double x2, double y2, double x3,
                          double y3);

double math_bezier_length(double x1, double y1, double x2, double y2, double x3,
                          double y3, double x4, double y4);

#ifdef __cplusplus
}
#endif

#endif //__MATH_PRIVATE_H__
