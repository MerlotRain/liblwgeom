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

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

/* Converts from radians to degrees. */
double math_to_degrees(double radians);

/* Converts from degrees to radians. */
double math_radians(double degrees);

/* Return the angle of the vector from (x1, y1) to (x2, y2). relative to the
 * positive x-axis. */
double math_angle(double x1, double y1, double x2, double y2);

/* Tests whether the angle between (x1, y1), (x2, y2), and (x3, y3) is acute.
 */
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

#endif
