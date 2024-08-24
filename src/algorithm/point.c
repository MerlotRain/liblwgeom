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
#include "nv-common.h"
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

/// @brief point angle with nv_point(0, 0)
/// @param p the point to compute the angle for
/// @return the angle of the point
double nv_angle(const struct nv_point p0)
{
    return atan2(p0.y, p0.x);
}

/// @brief Returns the angle of the vector from p0 to p1, relative to the
/// positive X-axis.
/// @param p0 the first point
/// @param p1 the second point
/// @return the angle of the vector from p0 to p1
double nv_angle2(const struct nv_point p0, const struct nv_point p1)
{
    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;
    return atan2(dy, dx);
}

/// @brief Tests whether the angle between p0-p1-p2 is acute.
/// @param p0 the first point
/// @param p1 the second point
/// @param p2 the third point
/// @return true if the angle is acute
bool nv_acute(const struct nv_point p0, const struct nv_point p1,
              const struct nv_point p2)
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
/// @return true if the angle is obtuse
bool nv_obtuse(const struct nv_point p0, const struct nv_point p1,
               const struct nv_point p2)
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
double nv_angle_between(const struct nv_point tip1, const struct nv_point tail,
                        const struct nv_point tip2)
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
bool nv_interior_angle(const struct nv_point p0, const struct nv_point p1,
                       const struct nv_point p2)
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
void nv_angle_bisector(const struct nv_point A, const struct nv_point B,
                       const struct nv_point C, const struct nv_point D,
                       struct nv_point *p, double *angle)
{
}

/// @brief Computes the distance from a point p to a line segment AB
/// @param p the point to compute the distance for
/// @param A one point of the line
/// @param B another point of the line (must be different to A)
/// @return the distance from p to line segment AB
double nv_dis_point_to_segment(const struct nv_point p, const struct nv_point A,
                               const struct nv_point B)
{
    if (NV_DOUBLE_NEARES2(A.x, B.x) && NV_DOUBLE_NEARES2(A.y, B.y)) {
        return NV_POINTDISTANCE2(p, A);
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
        return NV_POINTDISTANCE2(p, A);
    }
    if (r >= 1.0) {
        return NV_POINTDISTANCE2(p, B);
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
double nv_dis_point_to_perpendicular(const struct nv_point p,
                                     const struct nv_point A,
                                     const struct nv_point B)
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

void nv_segment_intersection(const struct nv_point p1, const struct nv_point p2,
                             const struct nv_point p3, const struct nv_point p4,
                             const struct nv_point *pin, bool *intersection)
{
}
