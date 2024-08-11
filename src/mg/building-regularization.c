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

#include "mgp.h"

struct bbr_seg {
    int index;
    double x0;
    double y0;
    double x1;
    double y1;
    double azimuth;
    double mx;
    double my;
    double dis;
};

void mg_building_regularization(double *xy, int np)
{
    assert(xy && np > 4);

    // rdp simplify
    double *rdp_xy = xy;
    int rdp_np = np;

    // segments
    struct bbr_seg *segs =
        (struct bbr_seg *)calloc(rdp_np - 1, sizeof(struct bbr_seg));
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