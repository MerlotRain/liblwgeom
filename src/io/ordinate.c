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

#include "ordinate.h"

struct Ordinate ordinate_XY()
{
    struct Ordinate flag;
    flag.value = ORDINATE_VALUE_X | ORDINATE_VALUE_Y;
    flag.changeAllowed = true;
    return flag;
}

void ordinate_setZ(struct Ordinate *o, bool v)
{
    if ((o->value & ORDINATE_VALUE_Z) != v) {
        if (o->changeAllowed) {
            o->value ^= ORDINATE_VALUE_Z;
        }
    }
}

void ordinate_setM(struct Ordinate *o, bool v)
{
    if ((o->value & ORDINATE_VALUE_M) != v) {
        if (o->changeAllowed) {
            o->value ^= ORDINATE_VALUE_M;
        }
    }
}