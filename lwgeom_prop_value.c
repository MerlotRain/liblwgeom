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

static double lwgeom__prop_area(const LWGEOM *obj)
{
    uint32_t rlen = obj->npoints;
    if (rlen < 3)
        return 0.0;

    double sum = 0.0;
    double x0 = obj->pp[0];
    for (size_t i = 1; i < rlen - 1; i++) {
        double x = lwgeom_get_x(obj, i) - x0;
        double y1 = lwgeom_get_y(obj, i + 1);
        double y2 = lwgeom_get_y(obj, i - 1);
        sum += x * (y2 - y1);
    }
    return (sum / 2.0);
}

static double lwgeom__prop_length(const LWGEOM *obj)
{
    size_t n = obj->npoints;
    if (n <= 1) {
        return 0.0;
    }
    double len = 0.0;
    double x0 = obj->pp[0];
    double y0 = obj->pp[1];
    for (int i = 1; i < n; ++i) {
        double x1 = lwgeom_get_x(obj, i);
        double y1 = lwgeom_get_y(obj, i);
        double dx = x1 - x0;
        double dy = y1 - y0;

        len += sqrt(dx * dx + dy * dy);
        x0 = x1;
        y0 = y1;
    }
    return len;
}

double lwgeom_prop_area(const LWGEOM *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = lwgeom__prop_area(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += lwgeom__prop_area(obj->geoms[i]);
        }
    }
    return sum;
}

double lwgeom_prop_length(const LWGEOM *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = lwgeom__prop_length(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += lwgeom__prop_length(obj->geoms[i]);
        }
    }
    return sum;
}

double lwgeom_prop_width(const LWGEOM *obj)
{
    assert(obj);
    double xmin = 0.0;
    double xmax = 0.0;
    if (obj->ngeoms == 1) {
        xmin = lwgeom_get_x(obj, 0);
        xmax = lwgeom_get_x(obj, 0);
        for (int i = 1; i < obj->npoints; ++i) {
            xmin = lwgeom_get_x(obj, i) > xmin ? xmin : lwgeom_get_x(obj, i);
            xmax = lwgeom_get_x(obj, i) < xmax ? xmax : lwgeom_get_x(obj, i);
        }
        return (xmax - xmin);
    }
    else {
        xmin = obj->geoms[0]->pp[0];
        xmax = obj->geoms[0]->pp[0];
        for (int i = 0; i < obj->ngeoms; ++i) {
            LWGEOM *sub = obj->geoms[i];
            if (sub == NULL)
                continue;
            for (int j = 0; j < sub->npoints; ++j) {
                xmin =
                    lwgeom_get_x(sub, i) > xmin ? xmin : lwgeom_get_x(sub, i);
                xmax =
                    lwgeom_get_x(sub, i) < xmax ? xmax : lwgeom_get_x(sub, i);
            }
        }
        return (xmax - xmin);
    }
}

double lwgeom_prop_height(const LWGEOM *obj)
{
    assert(obj);
    double ymin = 0.0;
    double ymax = 0.0;
    if (obj->ngeoms == 1) {
        ymin = lwgeom_get_y(obj, 0);
        ymax = lwgeom_get_y(obj, 0);
        for (int i = 1; i < obj->npoints; ++i) {
            ymin = lwgeom_get_y(obj, i) > ymin ? ymin : lwgeom_get_y(obj, i);
            ymax = lwgeom_get_y(obj, i) < ymax ? ymax : lwgeom_get_y(obj, i);
        }
        return (ymax - ymin);
    }
    else {
        ymin = obj->geoms[0]->pp[1];
        ymax = obj->geoms[0]->pp[1];
        for (int i = 0; i < obj->ngeoms; ++i) {
            LWGEOM *sub = obj->geoms[i];
            if (sub == NULL)
                continue;
            for (int j = 0; j < sub->npoints; ++j) {
                ymin =
                    lwgeom_get_y(obj, i) > ymin ? ymin : lwgeom_get_y(obj, i);
                ymax =
                    lwgeom_get_y(obj, i) < ymax ? ymax : lwgeom_get_y(obj, i);
            }
        }
        return (ymax - ymin);
    }
}