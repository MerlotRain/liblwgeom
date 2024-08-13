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

#include "mghelp.h"
#include "mgp.h"

static double pri_normalize_positive(double angle)
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

static double p_diff(double ang1, double ang2)
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

double mg_angle(const struct mg_point p0)
{
    return atan2(p0.y, p0.x);
}

double mg_angle2(const struct mg_point p0, const struct mg_point p1)
{
    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;
    return atan2(dy, dx);
}

bool mg_acute(const struct mg_point p0, const struct mg_point p1,
              const struct mg_point p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod > 0;
}

bool mg_obtuse(const struct mg_point p0, const struct mg_point p1,
               const struct mg_point p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod < 0;
}

double mg_angle_between(const struct mg_point tip1, const struct mg_point tail,
                        const struct mg_point tip2)
{
    double a1 = mg_angle2(tail, tip1);
    double a2 = mg_angle2(tail, tip2);

    return p_diff(a1, a2);
}

bool mg_interior_angle(const struct mg_point p0, const struct mg_point p1,
                       const struct mg_point p2)
{
    double angle_prev = mg_angle2(p1, p0);
    double angle_next = mg_angle2(p1, p2);
    return pri_normalize_positive(angle_next - angle_prev);
}

double mg_dis_point_to_segment(const struct mg_point p, const struct mg_point A,
                               const struct mg_point B)
{
    if (MG_DOUBLE_NEARES2(A.x, B.x) && MG_DOUBLE_NEARES2(A.y, B.y)) {
        return MG_POINTDISTANCE2(p, A);
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
        return MG_POINTDISTANCE2(p, A);
    }
    if (r >= 1.0) {
        return MG_POINTDISTANCE2(p, B);
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
double mg_dis_point_to_perpendicular(const struct mg_point p,
                                     const struct mg_point A,
                                     const struct mg_point B)
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

void mg_segment_intersection(const struct mg_point p1, const struct mg_point p2,
                             const struct mg_point p3, const struct mg_point p4,
                             const struct mg_point *pin, bool *intersection)
{
}

void mg_angle_bisector(const struct mg_point A, const struct mg_point B,
                       const struct mg_point C, const struct mg_point D,
                       struct mg_point *p, double *angle)
{
}