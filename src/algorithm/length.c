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

double _mg_prop_length_value(const struct nv_geobject *obj)
{
    size_t n = obj->npoints;
    if (n <= 1) {
        return 0.0;
    }
    double len = 0.0;
    double x0 = obj->pp[0];
    double y0 = obj->pp[1];
    for (int i = 1; i < n; ++i) {
        double x1 = obj->pp[(ptrdiff_t)(i * obj->cdim)];
        double y1 = obj->pp[(ptrdiff_t)((i * obj->cdim) + 1)];
        double dx = x1 - x0;
        double dy = y1 - y0;

        len += sqrt(dx * dx + dy * dy);
        x0 = x1;
        y0 = y1;
    }
    return len;
}

/// calc geometry length
double mg_prop_length_value(const struct nv_geobject *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = _mg_prop_length_value(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += _mg_prop_length_value(obj->objects[i]);
        }
    }
    return sum;
}