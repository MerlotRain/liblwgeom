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

double _mg_prop_area_value(const struct mg_object *obj)
{
    int rlen = obj->npoints;
    if (rlen < 3)
        return 0.0;

    double sum = 0.0;
    double x0 = obj->pp[0];
    for (size_t i = 1; i < rlen - 1; i++) {
        double x = obj->pp[i * obj->cdim] - x0;
        double y1 = obj->pp[(i + 1) * obj->cdim + 1];
        double y2 = obj->pp[(i - 1) * obj->cdim + 1];
        sum += x * (y2 - y1);
    }
    return (sum / 2.0);
}

/// calc geometry area
double mg_prop_area_value(const struct mg_object *obj)
{
    assert(obj);

    double sum = 0.0;
    if (obj->ngeoms == 0) {
        sum = _mg_prop_area_value(obj);
    }
    else {
        for (int i = 0; i < obj->ngeoms; ++i) {
            sum += _mg_prop_area_value(obj->objects[i]);
        }
    }
    return sum;
}