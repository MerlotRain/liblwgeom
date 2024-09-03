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

double nv__prop_length_value(const struct nv_geom *obj)
{
    size_t n = obj->npoints;
    if (n <= 1) {
        return 0.0;
    }
    double len = 0.0;
    double x0 = obj->pp[0];
    double y0 = obj->pp[1];
    for (int i = 1; i < n; ++i) {
        double x1 = nv__geo_get_x(obj, i);
        double y1 = nv__geo_get_y(obj, i);
        double dx = x1 - x0;
        double dy = y1 - y0;

        len += sqrt(dx * dx + dy * dy);
        x0 = x1;
        y0 = y1;
    }
    return len;
}

/// calc geometry length
double nv_prop_length_value(const struct nv_geom *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = nv__prop_length_value(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += nv__prop_length_value(obj->objects[i]);
        }
    }
    return sum;
}