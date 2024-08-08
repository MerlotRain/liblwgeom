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

/// calc geometry length
extern double geom_prop_length_value(const struct mg_object *obj);
/// calc geometry area
extern double geom_prop_area_value(const struct mg_object *obj);
/// calc geometry width
extern double geom_prop_width_value(const struct mg_object *obj);
/// calc geometry height
extern double geom_prop_height_value(const struct mg_object *obj);

double geom_prop_value(const struct mg_object *obj, int mode)
{
    assert(obj);
    switch (mode) {
    case GEOMETRY_PROP_VALUE_LENGTH: {
        return geom_prop_length_value(obj);
    }
    case GEOMETRY_PROP_VALUE_WIDTH: {
        return geom_prop_width_value(obj);
    }
    case GEOMETRY_PROP_VALUE_HEIGHT: {
        return geom_prop_height_value(obj);
    }
    case GEOMETRY_PROP_VALUE_AREA: {
        return geom_prop_area_value(obj);
    }
    default:
        return 0;
    }
}

struct mg_object *geom_prop_geo(const struct mg_object *obj, int mode)
{
}

void geom_prop_geo2(const struct mg_object *obj, int mode, double *paras)
{
}