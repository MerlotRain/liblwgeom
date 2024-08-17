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

#ifndef ORDINATE_H
#define ORDINATE_H

#include <mathse.h>

#ifdef __cpluscplus
extern "C" {
#endif

#define ORDINATE_VALUE_X (1)
#define ORDINATE_VALUE_Y (2)
#define ORDINATE_VALUE_Z (4)
#define ORDINATE_VALUE_M (8)

struct Ordinate {
    uint8_t value;
    bool changeAllowed; ///< allow change
};

struct Ordinate ordinate_XY();
void ordinate_setZ(struct Ordinate *o, bool v);
void ordinate_setM(struct Ordinate *o, bool v);

#ifdef __cpluscplus
}
#endif

#endif