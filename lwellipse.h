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

#ifndef LWELLIPSE_H
#define LWELLIPSE_H

#include "liblwgeom.h"

/// lwellipse_t is used to describe an ellipse or circle.
/// Before V1.0, it would be treated as a regular geometric shape and
/// temporarily not included in the unified management of the nv_geom model,
/// while providing relevant algorithms for circles or ellipses.
typedef struct {
    POINT2D center;
    double major;
    double minor;
    double azimuth;
} lwellipse_t;

enum {

    /// The eccentricity of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_ECCENTRICITY = 0,
    /// The area of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_AREA = 1,
    /// The perimeter of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_PERIMETER = 2,
    /// Two foci of the ellipse. The axes are oriented by the azimuth and are on
    /// the semi-major axis. - nv_point[2]
    NV_ELLIPSE_PROP_VALUE_FOCI = 4,
    /// The distance between the center and each foci. - double
    NV_ELLIPSE_PROP_FOCUS_DISTANCE = 8
};

enum {
    /// Two points form a circle, and the line segment between these two points
    /// is the diameter of the circle
    NV_CONSTRUCT_CIRCLE_2P,
    /// Three points form a circle, and these three points are on the circle
    NV_CONSTRUCT_CIRCLE_3P,
    /// To construct a circle with three tangent lines, six points need to be
    /// passed in. These six points form three straight lines, which can
    /// generate 0-2 circles. They are also the inscribed circles of a triangle
    NV_CONSTRUCT_CIRCLE_ICT
};

extern void lwellipse_construct_circle(const POINT2D *p, int t, lwellipse_t *es,
                                       int *n);
extern void lwellipse_prop_value(const lwellipse_t ell, int flags,
                                 double *values);
extern LWGEOM *lwellipse_stroke(lwellipse_t e, uint32_t param, LWBOOLEAN hasz,
                                LWBOOLEAN hasm);

#endif /* LWELLIPSE_H */