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

#include "geom-internal.h"
#include <math.h>
#include <string.h>

/// triangle inscribed circle
static struct nv_ellipse nv__tri_inscribed_circle(const struct nv_point2d p1,
                                                  const struct nv_point2d p2,
                                                  const struct nv_point2d p3)
{
    double l[3] = {NV_POINTDISTANCE2(p1, p2), NV_POINTDISTANCE2(p2, p3),
                   NV_POINTDISTANCE2(p3, p1)};
    double per = l[0] + l[1] + l[2];
    double x = (l[0] * p3.x + l[1] * p1.x + l[2] * p2.x) / per;
    double y = (l[0] * p3.y + l[1] * p1.y + l[2] * p2.y) / per;

    double r =
        fabs((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)) /
        per;
    struct nv_ellipse circle = {
        .center = {.x = x, .y = y}, .major = r, .minor = r, .azimuth = 0};
    return circle;
}

/// check cricle list contains one circle
static int nv__contains_circle(const struct nv_ellipse *es, int n,
                               const struct nv_point2d c, double r)
{
    for (int i = 0; i < n; ++i) {
        if (NV_POINTDISTANCE2(es[i].center, c) &&
            NV_DOUBLE_NEARES2(es[i].major, r)) {
            return NV_TRUE;
        }
    }
    return NV_FALSE;
}

/// Returns a new point which corresponds to this point projected by a specified
/// distance with specified angles
static struct nv_point2d nv__point_project(const struct nv_point2d p,
                                           double dis, double azimuth)
{
    struct nv_point2d pr;
    const double rads = azimuth * M_PI / 180.0;
    double dx = 0.0, dy = 0.0, dz = 0.0;

    dx = dis * sin(rads);
    dy = dis * cos(rads);
    pr.x = p.x + dx;
    pr.y = p.y + dy;

    return pr;
}

static double nv__normalized_angle(double angle)
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

static double nv__line_angle(double x1, double y1, double x2, double y2)
{
    double at = atan2(y2 - y1, x2 - x1);
    double a = -at + M_PI_2;
    return nv__normalized_angle(a);
}

static int nv__is_perpendicular(const struct nv_point2d pt1,
                                const struct nv_point2d pt2,
                                const struct nv_point2d pt3)
{
    double yDelta_a = pt2.y - pt1.y;
    double xDelta_a = pt2.x - pt1.x;
    double yDelta_b = pt3.y - pt2.y;
    double xDelta_b = pt3.x - pt2.x;

    if (NV_DOUBLE_NEARES(xDelta_a) && NV_DOUBLE_NEARES(yDelta_b)) {
        return NV_FALSE;
    }

    if (NV_DOUBLE_NEARES(yDelta_a)) {
        return NV_TRUE;
    }
    else if (NV_DOUBLE_NEARES(yDelta_b)) {
        return NV_TRUE;
    }
    else if (NV_DOUBLE_NEARES(xDelta_a)) {
        return NV_TRUE;
    }
    else if (NV_DOUBLE_NEARES(xDelta_b)) {
        return NV_TRUE;
    }
    return NV_FALSE;
}

static void nv__from_2parallels_line(const struct nv_point2d pt1_par1,
                                     const struct nv_point2d pt2_par1,
                                     const struct nv_point2d pt1_par2,
                                     const struct nv_point2d pt2_par2,
                                     const struct nv_point2d pt1_line1,
                                     const struct nv_point2d pt2_line1,
                                     struct nv_ellipse *rs, int *n)
{
    int _en = 0;
    const double radius =
        nv_dis_point_to_perpendicular(pt1_par1, pt1_par2, pt2_par2) / 2.0;

    int isInter;
    const struct nv_point2d ptInter;

    struct nv_point2d ptInter_par1line1, ptInter_par2line1;
    double angle1, angle2;
    double x, y;
    nv_angle_bisector(pt1_par1, pt2_par1, pt1_line1, pt2_line1,
                      &ptInter_par1line1, &angle1);

    nv_angle_bisector(pt1_par2, pt2_par2, pt1_line1, pt2_line1,
                      &ptInter_par2line1, &angle2);

    struct nv_point2d center;
    nv_segment_intersection(
        ptInter_par1line1, nv__point_project(ptInter_par1line1, 1.0, angle1),
        ptInter_par2line1, nv__point_project(ptInter_par2line1, 1.0, angle2),
        &center, &isInter);
    if (isInter) {
        _en++;
        *n = _en;
        rs[_en - 1].center = center;
        rs[_en - 1].major = radius;
        rs[_en - 1].minor = radius;
        rs[_en - 1].azimuth = 0;
    }

    nv_segment_intersection(
        ptInter_par1line1, nv__point_project(ptInter_par1line1, 1.0, angle1),
        ptInter_par2line1,
        nv__point_project(ptInter_par2line1, 1.0, angle2 + 90), &center,
        &isInter);
    if (isInter) {
        _en++;
        *n = _en;
        rs[_en - 1].center = center;
        rs[_en - 1].major = radius;
        rs[_en - 1].minor = radius;
        rs[_en - 1].azimuth = 0;
    }

    nv_segment_intersection(
        ptInter_par1line1,
        nv__point_project(ptInter_par1line1, 1.0, angle1 + 90),
        ptInter_par2line1, nv__point_project(ptInter_par2line1, 1.0, angle2),
        &center, &isInter);
    if (isInter && !nv__contains_circle(rs, *n, center, radius)) {
        _en++;
        *n = _en;
        rs[_en - 1].center = center;
        rs[_en - 1].major = radius;
        rs[_en - 1].minor = radius;
        rs[_en - 1].azimuth = 0;
    }

    nv_segment_intersection(
        ptInter_par1line1,
        nv__point_project(ptInter_par1line1, 1.0, angle1 + 90),
        ptInter_par2line1, nv__point_project(ptInter_par2line1, 1.0, angle2),
        &center, &isInter);
    if (isInter && !nv__contains_circle(rs, *n, center, radius)) {
        _en++;
        *n = _en;
        rs[_en - 1].center = center;
        rs[_en - 1].major = radius;
        rs[_en - 1].minor = radius;
        rs[_en - 1].azimuth = 0;
    }
}

/// @brief Construct circles according to different calculation methods.
///
/// Construct circles according to different calculation methods. \a t is
/// determined based on the \a NV_CONSTRUCT_CIRCLE series macros.
/// For NV_CONSTRUCT_CIRCLE_2P andNV_CONSTRUCT_CIRCLE_3P, a circle will
/// ultimately be generated.
/// When the value of \a n is NV_CONSTRUCT_CIRCLE_ICT, if \a n is -1, the
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
void nv_construct_circle(const struct nv_point2d *p, int t,
                         struct nv_ellipse *rs, int *n)
{
    assert(p);
    assert(rs);

    ///
    /// Constructs a circle by 2 points on the circle.
    /// The center point can have m value which is the result from the midpoint
    /// operation between \a pt1 and \a pt2. Z dimension is also supported and
    /// is retrieved from the first 3D point amongst \a pt1 and \a pt2.
    /// The radius is calculated from the 2D distance between \a pt1 and \a pt2.
    /// The azimuth is the angle between \a pt1 and \a pt2.
    /// @param pt1 First point.
    /// @param pt2 Second point.
    ///
    if (NV_CONSTRUCT_CIRCLE_2P == t) {
        struct nv_point2d pt1 = p[0];
        struct nv_point2d pt2 = p[1];

        struct nv_point2d center = {.x = ((pt1.x + pt2.x) / 2.0),
                                    .y = ((pt1.y + pt2.y) / 2.0)};
        double radius = sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) +
                             (pt1.y - pt2.y) * (pt1.y - pt2.y)) /
                        2;
        double azimuth =
            nv__line_angle(pt1.x, pt1.y, pt2.x, pt2.y) * 180.0 / M_PI;
        rs[0].center = center;
        rs[0].major = radius;
        rs[0].minor = radius;
        rs[0].azimuth = azimuth;
        *n = 1;
    }
    ///
    /// Constructs a circle by 3 points on the circle.
    /// M value is dropped for the center point.
    /// Z dimension is supported and is retrieved from the first 3D point
    /// amongst \a pt1, \a pt2 and \a pt3.
    /// The azimuth always takes the default value.
    /// If the points are colinear an empty circle is returned.
    /// @param pt1 First point.
    /// @param pt2 Second point.
    /// @param pt3 Third point.
    ///
    else if (NV_CONSTRUCT_CIRCLE_3P == t) {
        struct nv_point2d p1, p2, p3;
        struct nv_point2d pt1 = p[0];
        struct nv_point2d pt2 = p[1];
        struct nv_point2d pt3 = p[2];
        if (!nv__is_perpendicular(pt1, pt2, pt3)) {
            p1 = pt1;
            p2 = pt2;
            p3 = pt3;
        }
        else if (!nv__is_perpendicular(pt1, pt3, pt2)) {
            p1 = pt1;
            p2 = pt3;
            p3 = pt2;
        }
        else if (!nv__is_perpendicular(pt2, pt1, pt3)) {
            p1 = pt2;
            p2 = pt1;
            p3 = pt3;
        }
        else if (!nv__is_perpendicular(pt2, pt3, pt1)) {
            p1 = pt2;
            p2 = pt3;
            p3 = pt1;
        }
        else if (!nv__is_perpendicular(pt3, pt2, pt1)) {
            p1 = pt3;
            p2 = pt2;
            p3 = pt1;
        }
        else if (!nv__is_perpendicular(pt3, pt1, pt2)) {
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

        if (NV_DOUBLE_NEARES(xDelta_a) || NV_DOUBLE_NEARES(xDelta_b)) {
            *n = 0;
            return;
        }
        const double aSlope = yDelta_a / xDelta_a;
        const double bSlope = yDelta_b / xDelta_b;
        if ((NV_DOUBLE_NEARES(xDelta_a)) && (NV_DOUBLE_NEARES(yDelta_b))) {
            struct nv_point2d center;
            center.x = 0.5 * (p2.x + p3.x);
            center.y = 0.5 * (p1.y + p2.y);
            double radius = NV_POINTDISTANCE2(center, pt1);

            rs[0].center = center;
            rs[0].major = radius;
            rs[0].minor = radius;
            rs[0].azimuth = 0.0;
            *n = 1;
        }

        if (NV_DOUBLE_NEARES(aSlope - bSlope)) {
            *n = 0;
            return;
        }
        struct nv_point2d center;
        center.x = (aSlope * bSlope * (p1.y - p3.y) + bSlope * (p1.x + p2.x) -
                    aSlope * (p2.x + p3.x)) /
                   (2.0 * (bSlope - aSlope));
        center.y = -1.0 * (center.x - (p1.x + p2.x) / 2.0) / aSlope +
                   (p1.y + p2.y) / 2.0;
        double radius = NV_POINTDISTANCE2(center, pt1);
        rs[0].center = center;
        rs[0].major = radius;
        rs[0].minor = radius;
        rs[0].azimuth = 0.0;
        *n = 1;
    }
    ///
    /// Constructs a circle by 3 tangents on the circle (aka inscribed circle of
    /// a triangle). Z and m values are dropped for the center point. The
    /// azimuth always takes the default value.
    /// @param pt1_tg1 First point of the first tangent.
    /// @param pt2_tg1 Second point of the first tangent.
    /// @param pt1_tg2 First point of the second tangent.
    /// @param pt2_tg2 Second point of the second tangent.
    /// @param pt1_tg3 First point of the third tangent.
    /// @param pt2_tg3 Second point of the third tangent.
    /// @param epsilon Value used to compare point.
    /// @param pos Point to determine which circle use in case of multi return.
    /// If the solution is not unique and pos is an empty point, an empty circle
    /// is returned. -- This case happens only when two tangents are parallels.
    /// (since QGIS 3.18)
    ///
    else if (NV_CONSTRUCT_CIRCLE_ICT == t) {
        struct nv_point2d pt1_tg1 = p[0];
        struct nv_point2d pt2_tg1 = p[1];
        struct nv_point2d pt1_tg2 = p[2];
        struct nv_point2d pt2_tg2 = p[3];
        struct nv_point2d pt1_tg3 = p[4];
        struct nv_point2d pt2_tg3 = p[5];

        struct nv_point2d p1, p2, p3;
        int isIntersect_tg1tg2 = NV_FALSE;
        int isIntersect_tg1tg3 = NV_FALSE;
        int isIntersect_tg2tg3 = NV_FALSE;
        nv_segment_intersection(pt1_tg1, pt2_tg1, pt1_tg2, pt2_tg2, &p1,
                                &isIntersect_tg1tg2);
        nv_segment_intersection(pt1_tg1, pt2_tg1, pt1_tg3, pt2_tg3, &p2,
                                &isIntersect_tg1tg3);
        nv_segment_intersection(pt1_tg2, pt2_tg2, pt1_tg3, pt2_tg3, &p3,
                                &isIntersect_tg2tg3);

        if (!isIntersect_tg1tg2 &&
            !isIntersect_tg2tg3) // three lines are parallels
        {
            *n = 0;
            return;
        }

        if (!isIntersect_tg1tg2) {
            nv__from_2parallels_line(pt1_tg1, pt2_tg1, pt1_tg2, pt2_tg2,
                                     pt1_tg3, pt2_tg3, rs, n);
            return;
        }
        else if (!isIntersect_tg1tg3) {
            nv__from_2parallels_line(pt1_tg1, pt2_tg1, pt1_tg3, pt2_tg3,
                                     pt1_tg2, pt2_tg2, rs, n);
            return;
        }
        else if (!isIntersect_tg2tg3) {
            nv__from_2parallels_line(pt1_tg2, pt2_tg2, pt1_tg3, pt2_tg3,
                                     pt1_tg1, pt1_tg1, rs, n);
            return;
        }

        // 3 tangents are not parallels
        rs[0] = nv__tri_inscribed_circle(p1, p2, p3);
        *n = 1;
    }
}

/// @brief query ellipse attribute
///
/// Calculate the ellipse attribute. \a flags are a combination of
/// \a NV_ELLIPSE_PROP macro series. When passing in \a values externally, the
/// data structure needs to be organized by oneself. The algorithm will write
/// the calculation result in sequence based on the bits of the flag.
///
/// @param ell ellipse
/// @param flags query flag
/// @param values query result
/// @return
void nv_ellipse_prop_value(const struct nv_ellipse ell, int flags,
                           double *values)
{
    int pos = 0;
    assert(values);

    // calc eccentrucity
    if (flags & NV_ELLIPSE_PROP_VALUE_ECCENTRICITY) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis / ell.major;
        pos += 1;
    }

    // calc area
    if (flags & NV_ELLIPSE_PROP_VALUE_AREA) {
        values[pos] = M_PI * ell.major * ell.minor;
        pos += 1;
    }

    // calc perimeter
    if (flags & NV_ELLIPSE_PROP_VALUE_PERIMETER) {
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
    if (flags & NV_ELLIPSE_PROP_VALUE_FOCI) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        struct nv_point2d p1 = nv__point_project(ell.center, dis, ell.azimuth);
        struct nv_point2d p2 = nv__point_project(ell.center, -dis, ell.azimuth);
        memcpy(values + pos, &p1, sizeof(struct nv_point2d));
        memcpy(values + pos + 2, &p2, sizeof(struct nv_point2d));
        pos += 4;
    }

    // calc focus distance
    if (flags & NV_ELLIPSE_PROP_FOCUS_DISTANCE) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis;
        pos += 1;
    }
}

/// @brief stroke ellipse to nv_geom
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
/// @return nv_geom
struct nv_geom *nv_ellipse_stroke(struct nv_ellipse e, uint32_t param)
{
    if (param < 1)
        return NULL;

    size_t gdim = param / (size_t)pow(10, (size_t)log10(param));
    size_t nseg = param % (size_t)pow(10, (size_t)log10(param));
    if (gdim < 2 || gdim > 3)
        return NULL;
    if (nseg < 3)
        return NULL;

    double *pp = (double *)nv__calloc((nseg + 1) * 2, sizeof(double));
    if (pp == NULL) {
        return NULL;
    }

    struct nv_point2d qu = nv__point_project(e.center, e.major, e.azimuth);
    double az = atan2(qu.y - e.center.y, qu.x - e.center.x);

    for (size_t i = 0; i < nseg; ++i) {
        double t = (2 * M_PI - ((2 * M_PI) / nseg * i));
        pp[i * 2] = e.center.x + e.major * cos(t) * cos(az) -
                    e.minor * sin(t) * sin(az);
        pp[i * 2 + 1] = e.center.y + e.major * cos(t) * sin(az) +
                        e.minor * sin(t) * cos(az);
    }

    pp[nseg * 2] = pp[0];
    pp[nseg * 2 + 1] = pp[1];

    return NULL;
    // nv_geo_create_single(gdim, nseg + 1, 2, pp, 0);
}