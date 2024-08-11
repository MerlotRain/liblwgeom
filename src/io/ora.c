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

struct mg_sdo_elem_info {
    int sdo_starting_offset;
    int sdo_etype;
    int sdo_interpretation;
};

struct mg_object *mg_read_ora(const struct sdo_geometry sdo, int flag)
{
    int dimension = 2 > (sdo.sdo_gtype / 1000) ? 2 : (sdo.sdo_gtype / 1000);

    int tt = sdo.sdo_gtype % 100;
    if (tt > 0 && tt < 4) {
        struct mg_sdo_elem_info *pElemInfo =
            (struct mg_sdo_elem_info *)sdo.sdo_elem_info;
        for (int i = 0; i < sdo.sdo_elem_count / 3; i++) {
        }
    }
    else if (tt > 4 && tt < 8) {
        // multi mg_object
    }
    return NULL;
}

int mg_write_ora(const struct mg_object *obj, struct sdo_geometry* sdo)
{
    return 0;
}