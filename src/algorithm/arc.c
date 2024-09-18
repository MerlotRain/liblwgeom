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

static int nv__arc_center(POINT2D c0, POINT2D c1, POINT2D c2, POINT2D *center)
{
    if (c0.x == c2.x && c0.y == c2.y) {
        center->x = (c0.x + c1.x) / 2.0;
        center->y = (c0.y + c1.y) / 2.0;
        return 1;
    }

    POINT2D a = {.x = c1.x - c2.x, .y = c1.y - c2.y};
    POINT2D b = {.x = c2.x - c0.x, .y = c2.y - c0.y};
    POINT2D c = {.x = c0.x - c1.x, .y = c0.y - c1.y};

    double d1 = -(b.x * c.x + b.y * c.y);
    double d2 = -(c.x * a.x + c.y * a.y);
    double d3 = -(a.x * b.x + a.y * b.y);

    double e1 = d2 * d3;
    double e2 = d3 * d1;
    double e3 = d1 * d2;
    double e = e1 + e2 + e3;

    POINT2D G3 = {.x = c0.x + c1.x + c2.x, .y = c0.y + c1.y + c2.y};
    POINT2D H = {.x = (e1 * c0.x + e2 * c1.x + e3 * c2.x) / e,
                 .y = (e1 * c0.y + e2 * c1.y + e3 * c2.y) / e};

    center->x = 0.5 * (G3.x - H.x);
    center->y = 0.5 * (G3.y - H.y);
    return 1;
}

/// @brief stroke arc to nv_geom
/// @param arc arc
/// @param maxAngleStepSizeDegress max angle step size
/// @return nv_geom
LWGEOM *nv_arc_stroke(struct nv_arc arc, double maxAngleStepSizeDegress)
{
    double startAngle, alongAngle, endAngle;
    double cx, cy, radius;
    return 0;
}