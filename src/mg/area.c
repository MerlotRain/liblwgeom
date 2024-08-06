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

double pri_geom_ring_area(const struct mg_ring* ring)
{
    int rlen = ring->npoints;
    if(rlen < 3)
        return 0.0;

    double sum = 0.0;
    double x0 = ring->points[0].x;
    for(std::size_t i = 1; i < rlen - 1; i++) {
        double x = ring->points[i].x - x0;
        double y1 = ring->points[i + 1].y;
        double y2 = ring->points[i - 1].y;
        sum += x * (y2 - y1);
    }
    return fabs(sum / 2.0);
}

/// calc geometry area
double geom_prop_area_value(const struct mg_geom *geom)
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
            sum += geom_prop_area_value(mlt->geoms[i]);
        }
    }
    else if(geom->geomt == MG_POINT || geom->geomt == MG_PATH)
    {
        sum = 0.0;
    }
    else if(geom->geomt == MG_RING)
    {
        sum = pri_geom_ring_area((struct mg_ring*)geom->path);
    }
    else if(geom->geomt == MG_POLYGON)
    {
        sum += pri_geom_ring_area(geom->polygon->exterior);
        for(int i = 0; i < geom->polygon->nholes; ++i)
        {
            sum -= pri_geom_ring_area(geom->polygon->holes[i]);
        }
    }

    return sum;
}