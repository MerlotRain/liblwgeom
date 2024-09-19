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
#include <float.h>

typedef struct {
    int index;
    double x0;
    double y0;
    double x1;
    double y1;
    double azimuth;
    double mx;
    double my;
    double dis;
} bbr_seg;

/// @brief Make the rings formed by the building outlines extracted from the
/// city image rectangular
/// @param xy building outline coordinates, Points may be added or deleted
/// within the algorithm
/// @param np number of points
void lwgeom_building_regularization(double *xy, int np)
{
    assert(xy && np > 4);

    // rdp simplify
    double *rdp_xy = xy;
    int rdp_np = np;

    // segments
    bbr_seg *segs = (bbr_seg *)lwcalloc(rdp_np - 1, sizeof(bbr_seg));
    if (segs == NULL) {
        return;
    }
    int main_index = 1;
    double max_dis = DBL_MIN;
    for (int i = 1; i < rdp_np; ++i) {
        segs[i].index = i;
        segs[i].x0 = rdp_xy[(ptrdiff_t)((i - 1) * 2)];
        segs[i].y0 = rdp_xy[(ptrdiff_t)((i - 1) * 2 + 1)];
        segs[i].x1 = rdp_xy[(ptrdiff_t)(i * 2 + 1)];
        segs[i].y1 = rdp_xy[(ptrdiff_t)(i * 2 + 1)];
        segs[i].mx = (segs[i].x0 + segs[i].x1) / 2.0;
        segs[i].my = (segs[i].y0 + segs[i].y1) / 2.0;
        segs[i].dis = sqrt(pow(segs[i].x1 - segs[i].x0, 2) +
                           pow(segs[i].y1 - segs[i].y0, 2));
        segs[i].azimuth =
            atan2(segs[i].y1 - segs[i].y0, segs[i].x1 - segs[i].x0);
        // find max dis
        if (segs[i].dis > max_dis) {
            max_dis = segs[i].dis;
            main_index = i;
        }
    }

    double main_azimuth = segs[main_index].azimuth;
    for (int i = 1; i < rdp_np; ++i) {
        // rotate
    }

    xy[0] = 0;
}