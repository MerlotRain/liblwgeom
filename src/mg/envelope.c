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

double mg_prop_width_value(const struct mg_object *obj)
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
            struct mg_object *sub = obj->objects[i];
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

double mg_prop_height_value(const struct mg_object *obj)
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
            struct mg_object *sub = obj->objects[i];
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