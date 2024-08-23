/*****************************************************************************/
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "nv-common.h"

static int nv__arc_center(struct nv_point c0, struct nv_point c1,
                          struct nv_point c2, struct nv_point *center)
{
    if (c0.x == c2.x && c0.y == c2.y) {
        center->x = (c0.x + c1.x) / 2.0;
        center->y = (c0.y + c1.y) / 2.0;
        return 1;
    }

    struct nv_point a = {.x = c1.x - c2.x, .y = c1.y - c2.y};
    struct nv_point b = {.x = c2.x - c0.x, .y = c2.y - c0.y};
    struct nv_point c = {.x = c0.x - c1.x, .y = c0.y - c1.y};

    double d1 = -(b.x * c.x + b.y * c.y);
    double d2 = -(c.x * a.x + c.y * a.y);
    double d3 = -(a.x * b.x + a.y * b.y);

    double e1 = d2 * d3;
    double e2 = d3 * d1;
    double e3 = d1 * d2;
    double e = e1 + e2 + e3;

    struct nv_point G3 = {.x = c0.x + c1.x + c2.x, .y = c0.y + c1.y + c2.y};
    struct nv_point H = {.x = (e1 * c0.x + e2 * c1.x + e3 * c2.x) / e,
                         .y = (e1 * c0.y + e2 * c1.y + e3 * c2.y) / e};

    center->x = 0.5 * (G3.x - H.x);
    center->y = 0.5 * (G3.y - H.y);
    return 1;
}

struct nv_geobject *nv_stroke_arc(struct nv_arc arc,
                                  double maxAngleStepSizeDegress)
{
    double startAngle, alongAngle, endAngle;
    double cx, cy, radius;
    return 0;
}