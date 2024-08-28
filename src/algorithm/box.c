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
#include <float.h>
#include <math.h>

/// check box is null?
#define MV_BOX_NULL(B)                                                        \
    (isnan(B.min.x) && isnan(B.max.x) && isnan(B.min.y) && isnan(B.max.y)) || \
        (NV_DOUBLE_NEARES2(B.min.x, DBL_MAX) &&                               \
         NV_DOUBLE_NEARES2(B.min.y, DBL_MAX) &&                               \
         NV_DOUBLE_NEARES2(B.max.x, -DBL_MAX) &&                              \
         NV_DOUBLE_NEARES2(B.max.y, -DBL_MAX))

/// init box
#define MV_BOX_INIT(B)                                      \
    struct nv_box B = {.min = {.x = DBL_MIN, .y = DBL_MIN}, \
                       .max = {.x = DBL_MAX, .y = DBL_MAX}};

/// @brief Check two box is intersects
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return true if the two Envelopes intersect
bool nv_box_intersects(const struct nv_box env1, const struct nv_box env2)
{
    const double x1 = NV_MAX(env1.min.x, env2.min.x);
    const double x2 = NV_MIN(env1.max.x, env2.max.x);
    if (x1 > x2)
        return false;
    const double y1 = NV_MAX(env1.min.y, env2.min.y);
    const double y2 = NV_MIN(env1.max.y, env2.max.y);
    return y1 <= y2;
}

/// @brief Computes the intersection of two Envelopes
/// @param env1 the first Envelope
/// @param env2 the second Envelope
/// @return the intersection of the two Envelopes
struct nv_box nv_box_intersection(const struct nv_box env1,
                                  const struct nv_box env2)
{
    MV_BOX_INIT(b)
    if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2)) {
        return b;
    }

    if (nv_box_intersects(env1, env2)) {
        b.min.x = env1.min.x > env2.min.x ? env1.min.x : env2.min.x;
        b.min.y = env1.min.y > env2.min.y ? env1.min.y : env2.min.y;
        b.max.x = env1.max.x < env2.max.x ? env1.max.x : env2.max.x;
        b.max.y = env1.max.y < env2.max.y ? env1.max.y : env2.max.y;
    }
    return b;
}

/// @brief Enlarges the boundary of the Envelope so that it contains
/// @param env1 the original Envelope
/// @param env2 the Envelope to be contained
/// @return the enlarged Envelope
struct nv_box nv_box_union(const struct nv_box env1, const struct nv_box env2)
{
    MV_BOX_INIT(b)
    if (MV_BOX_NULL(env1) || MV_BOX_NULL(env2)) {
        return b;
    }
    b.min.x = NV_MIN(env1.min.x, env2.min.x);
    b.min.y = NV_MIN(env1.min.y, env2.min.y);
    b.max.x = NV_MIN(env1.max.x, env2.max.x);
    b.max.y = NV_MIN(env1.max.y, env2.max.y);
    return b;
}

/// @brief Convert nv_box object to nv_geobject
/// @param e the nv_box object
/// @param gdim the geometry dimension
/// @return the nv_geobject object
struct nv_geobject *nv_box_stroke(struct nv_box e, int gdim)
{
    return NULL; // TODO
}

#undef MV_BOX_NULL
#undef MV_BOX_INIT