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

double pri_geom_ring_length(const struct mg_ring* ring)
{
    size_t n = pts->npoints;
    if(n <= 1) {
        return 0.0;
    }
    double len = 0.0;
    double x0 = ring->points[0].x;
    double y0 = ring->points[0].y;
    for(int i = 1; i < n; ++i)
    {
        double x1 = ring->points[i].x;
        double y1 = ring->points[i].y;
        double dx = x1 - x0;
        double dy = y1 - y0;

        len += sqrt(dx * dx + dy * dy);        
        x0 = x1;
        y0 = y1;
    }
    return len;
}

/// calc geometry length
double geom_prop_length_value(const struct mg_object *geom)
{
    assert(geom);

    double sum = 0.0;
    if(geom->geomt >= MG_MULTIPOINT)
    {
        struct mg_multi* mlt = geom->geomt->multi;
        if(!mlt)
            return 0.0;

        for(int i = 0; i < mlt->ngeoms; ++i)
        {
            sum += geom_prop_length_value(mlt->geoms[i]);
        }
    }
    else if(geom->geomt == MG_POINT)
    {
        sum = 0.0;
    }
    else if(geom->geomt == MG_RING || geom->geomt == MG_PATH)
    {
        sum = pri_geom_ring_length((struct mg_ring*)geom->path);
    }
    else if(geom->geomt == MG_POLYGON)
    {
        sum += pri_geom_ring_length(geom->polygon->exterior);
        for(int i = 0; i < geom->polygon->nholes; ++i)
        {
            sum += pri_geom_ring_length(geom->polygon->holes[i]);
        }
    }

    return sum;
}