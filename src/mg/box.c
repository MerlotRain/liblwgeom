/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "mghelp.h"

/// check box is null?
#define MG_BOX_NULL(B)                                                        \
    (isnan(B.min.x) && isnan(B.max.x) && isnan(B.min.y) && isnan(B.max.y)) || \
        (DBL_NEAR(B.min.x, DBL_MAX) && DBL_NEAR(B.min.y, DBL_MAX) &&          \
         DBL_NEAR(B.max.x, -DBL_MAX) && DBL_NEAR(B.max.y, -DBL_MAX))

/// init box
#define MG_BOX_INIT(B)                                      \
    struct mg_box B = {.min = {.x = DBL_MIN, .y = DBL_MIN}, \
                       .max = {.x = DBL_MAX, .y = DBL_MAX}};

bool mg_box_intersects(const struct mg_box env1, const struct mg_box env2)
{
    const double x1 = MAX(env1.min.x, env2.min.x);
    const double x2 = MIN(env1.max.x, env2.max.x);
    if (x1 > x2)
        return false;
    const double y1 = MAX(env1.min.y, env2.min.y);
    const double y2 = MIN(env1.max.y, env2.max.y);
    return y1 <= y2;
}

struct mg_box mg_box_intersection(const struct mg_box env1,
                                  const struct mg_box env2)
{
    MG_BOX_INIT(b)
    if (MG_BOX_NULL(env1) || MG_BOX_NULL(env2)) {
        return b;
    }

    if (mg_box_intersects(env1, env2)) {
        b.min.x = env1.min.x > env2.min.x ? env1.min.x : env2.min.x;
        b.min.y = env1.min.y > env2.min.y ? env1.min.y : env2.min.y;
        b.max.x = env1.max.x < env2.max.x ? env1.max.x : env2.max.x;
        b.max.y = env1.max.y < env2.max.y ? env1.max.y : env2.max.y;
    }
    return b;
}

struct mg_box mg_box_union(const struct mg_box env1, const struct mg_box env2)
{
    MG_BOX_INIT(b)
    if (MG_BOX_NULL(env1) || MG_BOX_NULL(env2)) {
        return b;
    }
    b.min.x = MIN(env1.min.x, env2.min.x);
    b.min.y = MIN(env1.min.y, env2.min.y);
    b.max.x = MIN(env1.max.x, env2.max.x);
    b.max.y = MIN(env1.max.y, env2.max.y);
    return b;
}

struct mg_object *mg_stroke_box(struct mg_box e, int gdim)
{
}

#undef MG_BOX_NULL
#undef MG_BOX_INIT