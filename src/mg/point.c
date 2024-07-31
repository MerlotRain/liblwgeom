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

#include "mg.h"

static double p_normalize_positive(double angle)
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

struct mg_point mg_point_project(struct mg_point p, double dis, double azimuth)
{
    struct mg_point pr;
    const double rads = azimuth * M_PI / 180.0;
    double dx = 0.0, dy = 0.0, dz = 0.0;

    dx = dis * sin(rads);
    dy = dis * cos(rads);
    pr.x = p.x + dx;
    pr.y = p.y + dy;

    return pr;
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
    return p_normalize_positive(angle_next - angle_prev);
}