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

double nv__prop_area_value(const struct nv_geom *obj)
{
    uint32_t rlen = obj->npoints;
    if (rlen < 3)
        return 0.0;

    double sum = 0.0;
    double x0 = obj->pp[0];
    for (size_t i = 1; i < rlen - 1; i++) {
        double x = nv__geo_get_x(obj, i) - x0;
        double y1 = nv__geo_get_y(obj, i + 1);
        double y2 = nv__geo_get_y(obj, i - 1);
        sum += x * (y2 - y1);
    }
    return (sum / 2.0);
}

/// calc geometry area
double nv_prop_area_value(const struct nv_geom *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = nv__prop_area_value(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += nv__prop_area_value(obj->objects[i]);
        }
    }
    return sum;
}