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
#include "liblwgeom_internel.h"
#include <math.h>

static double nv__normalize_positive(double angle)
{
    if (angle < 0.0) {
        while (angle < 0.0) {
            angle += 2 * M_PI;
        }
        // in case round-off error bumps the value over
        if (angle >= 2 * M_PI) {
            angle = 0.0;
        }
    }
    else {
        while (angle >= 2 * M_PI) {
            angle -= 2 * M_PI;
        }
        // in case round-off error bumps the value under
        if (angle < 0.0) {
            angle = 0.0;
        }
    }
    return angle;
}

static double nv__diff(double ang1, double ang2)
{
    double delAngle;

    if (ang1 < ang2) {
        delAngle = ang2 - ang1;
    }
    else {
        delAngle = ang1 - ang2;
    }

    if (delAngle > M_PI) {
        delAngle = 2 * M_PI - delAngle;
    }

    return delAngle;
}

static double nv__azimuth(const POINT2D p1, const POINT2D p2)
{
    const double dx = p2.x - p1.x;
    const double dy = p2.y - p1.y;
    return (atan2(dx, dy) * 180.0 / M_PI);
}

static int nv__line_intersection(double p1x, double p1y, POINT2D v1, double p2x,
                                 double p2y, POINT2D v2, POINT2D *intersection)
{
    const double d = v1.y * v2.x - v1.x * v2.y;

    if (LW_DOUBLE_NEARES2(d, 0))
        return LW_FALSE;

    const double dx = p2x - p1x;
    const double dy = p2y - p1y;
    const double k = (dy * v2.x - dx * v2.y) / d;

    intersection->x = p1x + v1.x * k;
    intersection->y = p1y + v1.y * k;
    return LW_TRUE;
}

/// @brief point angle with nv_point(0, 0)
/// @param p the point to compute the angle for
/// @return the angle of the point
double nv_angle(const POINT2D p0)
{
    return atan2(p0.y, p0.x);
}

/// @brief Returns the angle of the vector from p0 to p1, relative to the
/// positive X-axis.
/// @param p0 the first point
/// @param p1 the second point
/// @return the angle of the vector from p0 to p1
double nv_angle2(const POINT2D p0, const POINT2D p1)
{
    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;
    return atan2(dy, dx);
}

/// @brief Tests whether the angle between p0-p1-p2 is acute.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return LW_TRUE if the angle is acute
int nv_acute(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod > 0;
}

/// @brief Tests whether the angle between p0-p1-p2 is obtuse.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return LW_TRUE if the angle is obtuse
int nv_obtuse(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod < 0;
}

/// @brief Returns the unoriented smallest angle between two vectors.
/// @param tip1 the first tip point
/// @param tail the tail point
/// @param tip2 the second tip point
/// @return the unoriented smallest angle between two vectors
double nv_angle_between(const POINT2D tip1, const POINT2D tail,
                        const POINT2D tip2)
{
    double a1 = nv_angle2(tail, tip1);
    double a2 = nv_angle2(tail, tip2);

    return nv__diff(a1, a2);
}

/// @brief Computes the interior angle between two segments of a ring.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return the interior angle between two segments of a ring
double nv_interior_angle(const POINT2D p0, const POINT2D p1, const POINT2D p2)
{
    double angle_prev = nv_angle2(p1, p0);
    double angle_next = nv_angle2(p1, p2);
    return nv__normalize_positive(angle_next - angle_prev);
}

/// @brief The bisector segment of AB-CD is (point, projection of point by \a
/// angle)
/// @param A
/// @param B
/// @param C
/// @param D
/// @param p
/// @param angle
/// @return
int nv_angle_bisector(const POINT2D A, const POINT2D B, const POINT2D C,
                      const POINT2D D, POINT2D *p, double *angle)
{

    *angle = (nv__azimuth(A, B) + nv__azimuth(C, D)) / 2.0;

    int intersection = LW_FALSE;
    nv_segment_intersection(A, B, C, D, p, &intersection);
    return intersection;
}

/// @brief Computes the distance from a point p to a line segment AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
double nv_dis_point_to_segment(const POINT2D p, const POINT2D A,
                               const POINT2D B)
{
    if (LW_DOUBLE_NEARES2(A.x, B.x) && LW_DOUBLE_NEARES2(A.y, B.y)) {
        return LW_POINTDISTANCE2(p, A);
    }

    /*
        (1)
                        AC dot AB
                    r = ---------
                        ||AB||^2

        r has the following meaning:
        r=0 P = A
        r=1 P = B
        r<0 P is on the backward extension of AB
        r>1 P is on the forward extension of AB
        0<r<1 P is interior to AB
    */

    double r = ((p.x - A.x) * (B.x - A.x) + (p.y - A.y) * (B.y - A.y)) /
               ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));

    if (r <= 0.0) {
        return LW_POINTDISTANCE2(p, A);
    }
    if (r >= 1.0) {
        return LW_POINTDISTANCE2(p, B);
    }

    /*
        (2)
                (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
            s = -----------------------------
                            L^2

        Then the distance from C to P = |s|*L.
    */

    double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y)) /
               ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));

    return fabs(s) *
           sqrt(((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y)));
    return 0;
}

/// @brief Computes the perpendicular distance from a point p to the (infinite)
/// line containing the points AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
double nv_dis_point_to_perpendicular(const POINT2D p, const POINT2D A,
                                     const POINT2D B)
{
    /*
                (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
            s = -----------------------------
                                 L^2

            Then the distance from C to P = |s|*L.
    */

    double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y)) /
               ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
    return fabs(s) *
           sqrt(((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y)));
}

void nv_segment_intersection(const POINT2D p1, const POINT2D p2,
                             const POINT2D p3, const POINT2D p4, POINT2D *pin,
                             int *intersection)
{
    *intersection = LW_FALSE;
    double vl = LW_POINTDISTANCE2(p1, p2);
    double wl = LW_POINTDISTANCE2(p3, p4);

    if (LW_DOUBLE_NEARES2(vl, 0.0) || LW_DOUBLE_NEARES2(wl, 0.0))
        return;

    POINT2D v = {.x = (p2.x - p1.x) / vl, .y = (p2.y - p1.y) / vl};
    POINT2D w = {.x = (p4.x - p3.x) / wl, .y = (p4.y - p3.y) / wl};

    if (!nv__line_intersection(p1.x, p1.y, v, p3.x, p3.y, w, pin)) {
        return;
    }

    *intersection = LW_TRUE;
}