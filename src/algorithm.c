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
extern double geom_prop_length_value(const struct mg_geom *geom);
/// calc geometry area
extern double geom_prop_area_value(const struct mg_geom *geom);

double geom_prop_value(const struct mg_geom *geom, int mode)
{
    assert(geom);
    switch (mode) {
    case GEOMETRY_PROP_VALUE_LENGTH:
        return geom_prop_length_value(geom);
    case GEOMETRY_PROP_VALUE_WIDTH: {
        return 0.0;
    }
    case GEOMETRY_PROP_VALUE_HEIGHT: {
        return 0.0;
    }
    case GEOMETRY_PROP_VALUE_AREA:
        return geom_prop_area_value(geom);
    default:
        return 0;
    }
}

struct mg_geom *geom_prop_geo(const struct mg_geom *geom, int mode)
{
}

void geom_prop_geo2(const struct mg_geom *geom, int mode, double *paras)
{
}