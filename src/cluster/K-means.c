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

/// @brief K-means clustering
///
/// W only records the i4 attributes of P that conform to the classification.
/// For example, if the i4 attribute of each object is the object ID, then the
/// attribute in W is the ID of the classified object. Therefore, when calling
/// this interface, the i4 attribute in P should be identifiable.
///
/// @param P input data
/// @param n number of clusters
/// @param W output data
void nv_kmeans(struct nv_geo_reader2 *P, int n, struct nv_geo_reader2 **W)
{
    assert(P);
    assert(n > 1);

    struct nv_i4 *obj = nv_geo_reader2_iterator(P);
    while (obj) {
        double point[2] = {0};
        nv_prop_geo2(obj->obj, NV_GEOMETRY_PROP_GEO_CENTROID, point);
        obj = nv_geo_reader2_iterator(P);
    }
}