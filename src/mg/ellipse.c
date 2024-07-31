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

extern double tolerance();

static double pir_normalized_angle(double angle)
{
    double clippedAngle = angle;
    if (clippedAngle >= M_PI * 2 || clippedAngle <= -2 * M_PI) {
        clippedAngle = fmod(clippedAngle, 2 * M_PI);
    }
    if (clippedAngle < 0.0) {
        clippedAngle += 2 * M_PI;
    }
    return clippedAngle;
}

static double pir_line_angle(double x1, double y1, double x2, double y2)
{
    double at = atan2(y2 - y1, x2 - x1);
    double a = -at + M_PI_2;
    return pir_normalized_angle(a);
}

static bool pri_is_perpendicular(const struct mg_point pt1,
                                 const struct mg_point pt2,
                                 const struct mg_point pt3)
{
    double yDelta_a = pt2.y - pt1.y;
    double xDelta_a = pt2.x - pt1.x;
    double yDelta_b = pt3.y - pt2.y;
    double xDelta_b = pt3.x - pt2.x;

    if (fabs(xDelta_a) <= tolerance() && fabs(yDelta_b) <= tolerance()) {
        return false;
    }

    if (fabs(yDelta_a) <= tolerance()) {
        return true;
    }
    else if (fabs(yDelta_b) <= tolerance()) {
        return true;
    }
    else if (fabs(xDelta_a) <= tolerance()) {
        return true;
    }
    else if (fabs(xDelta_b) <= tolerance()) {
        return true;
    }
    return false;
}

void mg_ellipse_prop_value(const struct mg_ellipse ell, int flags,
                           double *values)
{
    int pos = 0;
    assert(values);

    // calc eccentrucity
    if (flags & MG_ELLIPSE_PROP_VALUE_ECCENTRICITY) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis / ell.major;
        pos += 1;
    }

    // calc area
    if (flags & MG_ELLIPSE_PROP_VALUE_AREA) {
        values[pos] = M_PI * ell.major * ell.minor;
        pos += 1;
    }

    // calc perimeter
    if (flags & MG_ELLIPSE_PROP_VALUE_PERIMETER) {
        if (ell.major == ell.minor) {
            values[pos] = M_PI * 2.0 * ell.major;
        }
        else {
            values[pos] =
                M_PI *
                (3 * (ell.major + ell.minor) -
                 sqrt(10 * ell.major * ell.minor +
                      3 * (ell.major * ell.major + ell.minor * ell.minor)));
        }
        pos += 1;
    }

    // calc foci
    if (flags & MG_ELLIPSE_PROP_VALUE_FOCI) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        struct mg_point p1 = mg_point_project(ell.center, dis, ell.azimuth);
        struct mg_point p2 = mg_point_project(ell.center, -dis, ell.azimuth);
        memcpy(values + pos, &p1, sizeof(struct mg_point));
        memcpy(values + pos + 2, &p2, sizeof(struct mg_point));
        pos += 4;
    }

    // calc focus distance
    if (flags & MG_ELLIPSE_PROP_FOCUS_DISTANCE) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis;
        pos += 1;
    }
}

void mg_construct_circle(const struct mg_point *p, int t, struct mg_ellipse *rs,
                         int *n)
{
    assert(p);
    assert(rs);
    if (MG_CONSTRUCT_CIRCLE_2P == t) {
        struct mg_point pt1 = p[0];
        struct mg_point pt2 = p[1];

        struct mg_point center = {.x = ((pt1.x + pt2.x) / 2.0),
                                  .y = ((pt1.y + pt2.y) / 2.0)};
        double radius = sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) +
                             (pt1.y - pt2.y) * (pt1.y - pt2.y));
        double azimuth =
            pir_line_angle(pt1.x, pt1.y, pt2.x, pt2.y) * 180.0 / M_PI;
        rs[0].center = center;
        rs[0].major = radius;
        rs[0].minor = radius;
        rs[0].azimuth = azimuth;
        *n = 1;
    }
    else if (MG_CONSTRUCT_CIRCLE_3P == t) {
        struct mg_point p1, p2, p3;
        struct mg_point pt1 = p[0];
        struct mg_point pt2 = p[1];
        struct mg_point pt3 = p[2];
        if (!pri_is_perpendicular(pt1, pt2, pt3)) {
            p1 = pt1;
            p2 = pt2;
            p3 = pt3;
        }
        else if (!pri_is_perpendicular(pt1, pt3, pt2)) {
            p1 = pt1;
            p2 = pt3;
            p3 = pt2;
        }
        else if (!pri_is_perpendicular(pt2, pt1, pt3)) {
            p1 = pt2;
            p2 = pt1;
            p3 = pt3;
        }
        else if (!pri_is_perpendicular(pt2, pt3, pt1)) {
            p1 = pt2;
            p2 = pt3;
            p3 = pt1;
        }
        else if (!pri_is_perpendicular(pt3, pt2, pt1)) {
            p1 = pt3;
            p2 = pt2;
            p3 = pt1;
        }
        else if (!pri_is_perpendicular(pt3, pt1, pt2)) {
            p1 = pt3;
            p2 = pt1;
            p3 = pt2;
        }
        else {
            *n = 0;
            return;
        }
        const double yDelta_a = p2.y - p1.y;
        const double xDelta_a = p2.x - p1.x;
        const double yDelta_b = p3.y - p2.y;
        const double xDelta_b = p3.x - p2.x;

        if (fabs(xDelta_a) < tolerance() || fabs(xDelta_b) < tolerance()) {
            *n = 0;
            return;
        }
        const double aSlope = yDelta_a / xDelta_a;
        const double bSlope = yDelta_b / xDelta_b;
        if ((fabs(xDelta_a) <= tolerance()) &&
            (fabs(yDelta_b) <= tolerance())) {
            struct mg_point center;
            center.x = 0.5 * (p2.x + p3.x);
            center.y = 0.5 * (p1.y + p2.y);
            double radius = sqrt((center.x - pt1.x) * (center.x - pt1.x) +
                                 (center.y - pt1.y) * (center.y - pt1.y));

            rs[0].center = center;
            rs[0].major = radius;
            rs[0].minor = radius;
            rs[0].azimuth = 0.0;
            *n = 1;
        }

        if (fabs(aSlope - bSlope) <= tolerance()) {
            *n = 0;
            return;
        }
        struct mg_point center;
        center.x = (aSlope * bSlope * (p1.y - p3.y) + bSlope * (p1.x + p2.x) -
                    aSlope * (p2.x + p3.x)) /
                   (2.0 * (bSlope - aSlope));
        center.y = -1.0 * (center.x - (p1.x + p2.x) / 2.0) / aSlope +
                   (p1.y + p2.y) / 2.0;
        double radius = sqrt((center.x - pt1.x) * (center.x - pt1.x) +
                             (center.y - pt1.y) * (center.y - pt1.y));
        rs[0].center = center;
        rs[0].major = radius;
        rs[0].minor = radius;
        rs[0].azimuth = 0.0;
        *n = 1;
    }
    else if (MG_CONSTRUCT_CIRCLE_ICT == t) {
    }
}