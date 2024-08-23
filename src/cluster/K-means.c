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

#include "nvp.h"

void mg_kmeans(struct nv_reader2 *P, int n, struct nv_reader2 **W)
{
    assert(P);
    assert(n > 1);

    struct nv_i4 *obj = mg_reader2_iterator(P);
    while (obj) {
        double point[2] = {0};
        mg_prop_geo2(obj->obj, GEOMETRY_PROP_GEO_CENTROID, point);
        obj = mg_reader2_iterator(P);
    }
}