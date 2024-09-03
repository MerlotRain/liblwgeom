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

double nv_prop_width_value(const struct nv_geom *obj)
{
    assert(obj);
    double xmin = 0.0;
    double xmax = 0.0;
    if (obj->ngeoms == 1) {
        xmin = nv__geo_get_x(obj, 0);
        xmax = nv__geo_get_x(obj, 0);
        for (int i = 1; i < obj->npoints; ++i) {
            xmin = nv__geo_get_x(obj, i) > xmin ? xmin : nv__geo_get_x(obj, i);
            xmax = nv__geo_get_x(obj, i) < xmax ? xmax : nv__geo_get_x(obj, i);
        }
        return (xmax - xmin);
    }
    else {
        xmin = obj->objects[0]->pp[0];
        xmax = obj->objects[0]->pp[0];
        for (int i = 0; i < obj->ngeoms; ++i) {
            struct nv_geom *sub = obj->objects[i];
            if (sub == NULL)
                continue;
            for (int j = 0; j < sub->npoints; ++j) {
                xmin =
                    nv__geo_get_x(sub, i) > xmin ? xmin : nv__geo_get_x(sub, i);
                xmax =
                    nv__geo_get_x(sub, i) < xmax ? xmax : nv__geo_get_x(sub, i);
            }
        }
        return (xmax - xmin);
    }
}

double nv_prop_height_value(const struct nv_geom *obj)
{
    assert(obj);
    double ymin = 0.0;
    double ymax = 0.0;
    if (obj->ngeoms == 1) {
        ymin = nv__geo_get_y(obj, 0);
        ymax = nv__geo_get_y(obj, 0);
        for (int i = 1; i < obj->npoints; ++i) {
            ymin = nv__geo_get_y(obj, i) > ymin ? ymin : nv__geo_get_y(obj, i);
            ymax = nv__geo_get_y(obj, i) < ymax ? ymax : nv__geo_get_y(obj, i);
        }
        return (ymax - ymin);
    }
    else {
        ymin = obj->objects[0]->pp[1];
        ymax = obj->objects[0]->pp[1];
        for (int i = 0; i < obj->ngeoms; ++i) {
            struct nv_geom *sub = obj->objects[i];
            if (sub == NULL)
                continue;
            for (int j = 0; j < sub->npoints; ++j) {
                ymin =
                    nv__geo_get_y(obj, i) > ymin ? ymin : nv__geo_get_y(obj, i);
                ymax =
                    nv__geo_get_y(obj, i) < ymax ? ymax : nv__geo_get_y(obj, i);
            }
        }
        return (ymax - ymin);
    }
}