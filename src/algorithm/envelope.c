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

#include "nv-common.h"

double nv_prop_width_value(const struct nv_geom *obj)
{
    assert(obj);
    double xmin = 0.0;
    double xmax = 0.0;
    if (obj->ngeoms == 1) {
        xmin = obj->pp[0];
        xmax = obj->pp[0];
        for (int i = 1; i < obj->npoints; ++i) {
            xmin =
                obj->pp[i * obj->cdim] > xmin ? xmin : obj->pp[i * obj->cdim];
            xmax =
                obj->pp[i * obj->cdim] < xmax ? xmax : obj->pp[i * obj->cdim];
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
                xmin = sub->pp[i * sub->cdim] > xmin ? xmin
                                                     : sub->pp[i * sub->cdim];
                xmax = sub->pp[i * sub->cdim] < xmax ? xmax
                                                     : sub->pp[i * sub->cdim];
            }
        }
        return (xmax - xmin);
    }
}

double nv_prop_height_value(const struct nv_geom *obj)
{
    assert(obj);
    double xmin = 0.0;
    double xmax = 0.0;
    if (obj->ngeoms == 1) {
        xmin = obj->pp[1];
        xmax = obj->pp[1];
        for (int i = 1; i < obj->npoints; ++i) {
            xmin = obj->pp[i * obj->cdim + 1] > xmin
                       ? xmin
                       : obj->pp[i * obj->cdim + 1];
            xmax = obj->pp[i * obj->cdim + 1] < xmax
                       ? xmax
                       : obj->pp[i * obj->cdim] + 1;
        }
        return (xmax - xmin);
    }
    else {
        xmin = obj->objects[0]->pp[1];
        xmax = obj->objects[0]->pp[1];
        for (int i = 0; i < obj->ngeoms; ++i) {
            struct nv_geom *sub = obj->objects[i];
            if (sub == NULL)
                continue;
            for (int j = 0; j < sub->npoints; ++j) {
                xmin = sub->pp[i * sub->cdim + 1] > xmin
                           ? xmin
                           : sub->pp[i * sub->cdim + 1];
                xmax = sub->pp[i * sub->cdim + 1] < xmax
                           ? xmax
                           : sub->pp[i * sub->cdim + 1];
            }
        }
        return (xmax - xmin);
    }
}