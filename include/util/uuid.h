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

#ifndef UUID_H
#define UUID_H

#include "mathse.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UUID_LEN 37

EXTERN int uuid_init();
EXTERN void uuid_generate(char *dst);

#ifdef __cplusplus
}
#endif

#endif