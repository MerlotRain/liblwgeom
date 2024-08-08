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

#ifndef MGP_H
#define MGP_H

#include "mg.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mg_path {
    int gdim;
    int npoints;
    int flag;
    double *pp;
};

struct mg_object {
    int gdim;
    int ngeoms;
    struct mg_path* path;
    struct mg_object** objects;
};

#ifdef __cplusplus
}
#endif

#endif