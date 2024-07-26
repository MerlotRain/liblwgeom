/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/


#include "math_util.h"
#include <math.h>

double math_to_degrees(double radians) { return radians * 180.0 / M_PI; }

double math_radians(double degrees) { return degrees * M_PI / 180.0; }

double math_angle(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return atan2(dy, dx);
}

int math_is_acute(
    double x1, double y1, double x2, double y2, double x3, double y3) {
    double dx0 = x1 - x2;
    double dy0 = y1 - y2;
    double dx1 = x3 - x2;
    double dy1 = y3 - y2;
    return dx0 * dx1 + dy0 * dy1 > 0;
}

double math_distance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

double math_ccw_angle(double dy, double dx) {
    const double angle = atan2(dy, dx) * 180 / M_PI;
    if (angle < 0) {
        return 360 + angle;
    } else if (angle > 360) {
        return 360 - angle;
    }
    return angle;
}

double math_sweep_angle(double cx,
                        double cy,
                        double x1,
                        double y1,
                        double x2,
                        double y2,
                        double x3,
                        double y3) {
    const double p1Angle = math_ccw_angle(y1 - cy, x1 - cx);
    const double p2Angle = math_ccw_angle(y2 - cy, x2 - cx);
    const double p3Angle = math_ccw_angle(y3 - cy, x3 - cx);

    if (p3Angle >= p1Angle) {
        if (p2Angle > p1Angle && p2Angle < p3Angle) {
            return (p3Angle - p1Angle);
        } else {
            return (-(p1Angle + (360 - p3Angle)));
        }
    } else {
        if (p2Angle < p1Angle && p2Angle > p3Angle) {
            return (-(p1Angle - p3Angle));
        } else {
            return (p3Angle + (360 - p1Angle));
        }
    }
}

void math_circle_center_radius(double  x1,
                               double  y1,
                               double  x2,
                               double  y2,
                               double  x3,
                               double  y3,
                               double *radius,
                               double *cx,
                               double *cy) {
    double dx21, dy21, dx31, dy31, h21, h31, d;

    if (x1 == x3 && y1 == y3) {
        *cx     = (x1 + x2) / 2.0;
        *cy     = (y1 + y2) / 2.0;
        *radius = sqrt(pow(*cx - x1, 2.0) + pow(*cy - y1, 2.0));
        return;
    }

    // https://en.wikipedia.org/wiki/Circumscribed_circle
    dx21 = x2 - x1;
    dy21 = y2 - y1;
    dx31 = x3 - x1;
    dy31 = y3 - y1;

    h21 = pow(dx21, 2.0) + pow(dy21, 2.0);
    h31 = pow(dx31, 2.0) + pow(dy31, 2.0);

    // 2*Cross product, d<0 means clockwise and d>0 counterclockwise sweeping
    // angle
    d = 2 * (dx21 * dy31 - dx31 * dy21);

    if (fabs(d) == 0.0) {
        *radius = -1.0;
        return;
    }

    *cx     = x1 + (h21 * dy31 - h31 * dy21) / d;
    *cy     = y1 - (h21 * dx31 - h31 * dx21) / d;
    *radius = sqrt(pow(*cx - x1, 2.0) + pow(*cy - y1, 2.0));
}

double math_circle_length(
    double x1, double y1, double x2, double y2, double x3, double y3) {
    double cx;
    double cy;
    double radius;
    math_circle_center_radius(x1, y1, x2, y2, x3, y3, &radius, &cx, &cy);
    double length = M_PI / 180.0 * radius *
                    math_sweep_angle(cx, cy, x1, y1, x2, y2, x3, y3);
    if (length < 0) {
        length = -length;
    }
    return length;
}

static void __math_bezier_split(bezier_t *bezier, bezier_t *halves) {
    assert(bezier && halves);
    raw_point_t mid_12 = mid_point(bezier->p1, bezier->p2);
    raw_point_t mid_23 = mid_point(bezier->p2, bezier->p3);
    raw_point_t mid_34 = mid_point(bezier->p3, bezier->p4);

    raw_point_t mid_12_23 = {.x = (mid_12.x + mid_23.x) * 0.5,
                             .y = (mid_12.y + mid_23.y) * 0.5};
    raw_point_t mid_23_34 = {.x = (mid_23.x + mid_34.x) * 0.5,
                             .y = (mid_23.y + mid_34.y) * 0.5};

    raw_point_t mid_12_23__23_34 = {.x = (mid_12_23.x + mid_23_34.x) * 0.5,
                                    .y = (mid_12_23.y + mid_23_34.y) * 0.5};

    halves[0].p1 = bezier->p1;
    halves[0].p2 = mid_12;
    halves[0].p3 = mid_12_23;
    halves[0].p4 = mid_12_23__23_34;

    halves[1].p1 = mid_12_23__23_34;
    halves[1].p2 = mid_23_34;
    halves[1].p3 = mid_34;
    halves[1].p4 = bezier->p4;
}

static void
__math_bezier_add_if_close(bezier_t *bezier, double *length, double error) {
    double len = 0.0;
    double chord;

    len +=
        math_distance(bezier->p1.x, bezier->p1.y, bezier->p2.x, bezier->p2.y);
    len +=
        math_distance(bezier->p2.x, bezier->p2.y, bezier->p3.x, bezier->p3.y);
    len +=
        math_distance(bezier->p3.x, bezier->p3.y, bezier->p4.x, bezier->p4.y);

    chord =
        math_distance(bezier->p1.x, bezier->p1.y, bezier->p4.x, bezier->p4.y);

    if ((len - chord) > error) {
        bezier_t halves[2];
        __math_bezier_split(bezier, halves);
        __math_bezier_add_if_close(&(halves[0]), length, error);
        __math_bezier_add_if_close(&(halves[1]), length, error);
        return;
    }
    *length = *length + len;
}

double math_bezier_length(double x1,
                          double y1,
                          double x2,
                          double y2,
                          double x3,
                          double y3,
                          double x4,
                          double y4) {

    bezier_t bezier = {.p1 = {.x = x1, .y = y1},
                       .p2 = {.x = x2, .y = y2},
                       .p3 = {.x = x3, .y = y3},
                       .p4 = {.x = x4, .y = y4}};

    double length = 0.0;
    __math_bezier_add_if_close(&bezier, &length, 0.01);

    return length;
}