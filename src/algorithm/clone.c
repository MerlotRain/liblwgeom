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

struct nv_geobject *mg_prop_geo_clone(const struct nv_geobject *obj)
{
    assert(obj);

    struct nv_geobject *clone =
        (struct nv_geobject *)malloc(sizeof(struct nv_geobject));
    if (clone == NULL) {
        return NULL;
    }

    clone->ngeoms = obj->ngeoms;
    if (obj->ngeoms == 1) {
    }
    return NULL;
}