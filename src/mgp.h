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
#include "rtree.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Create a multi-geometry or single-geometry object
struct mg_object {
    int ngeoms;                 ///< number of geometries
    int gdim;                   ///< geometry dimension 0:point, 1:line, 2:area
    int cdim;                   ///< coordinate dimension 2:2D, 3:3D
    int npoints;                ///< number of points
    int flag;                   ///< flag 0: reference input, 1: memory copy
    int clockwise;              ///< 1: clockwise, 0: counterclockwise
    double *pp;                 ///< point pointer
    struct mg_object **objects; ///< multi objects pointer
};

/// @brief create integer attribute object
struct mg_i4 {
    int id;                ///< i4 object id
    struct mg_object *obj; ///< geometry object
    int propSize;          ///< property size
    int *prop;             ///< property array
};

struct mg_reader2 {
    int current;         ///< current index
    int nobj;            ///< number of objects
    struct mg_i4 **objs; ///< object array
    struct rtree *index; ///< spatial index
};

#ifdef __cplusplus
}
#endif

#endif