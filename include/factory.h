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

#ifndef FACTORY_H
#define FACTORY_H

#ifdef __cpluscplus
extern "C" {
#endif

#include <mathse.h>

/**
 * Geometric models, supporting common geometric models such as point, path,
 * ring, polyline, polygon, multipoint, etc.
 */
typedef struct SEGeom_t se_geom;

#define GEOMETRY_IO_TYPE_WKT     0
#define GEOMETRY_IO_TYPE_WKB     1
#define GEOMETRY_IO_TYPE_GEOJSON 2
#define GEOMETRY_IO_TYPE_EWKT    3
#define GEOMETRY_IO_TYPE_EWKB    4
#define GEOMETRY_IO_TYPE_KML     5
#define GEOMETRY_IO_TYPE_GML     6
#define GEOMETRY_IO_TYPE_GML2    7

/**
 * @brief read the geometry from the data
 * @param data the data of the geometry
 * @param len If it is binary data, \a len represents the length of the data.
 * If it is text data, \a len can be 0.
 * @param type type of data
 */
EXTERN se_geom *geom_read(const char *data, int len, int type);

EXTERN void geome_write(const se_geom *geom, char **data, int *len, int type,
                        int flag);

EXTERN void geom_free(se_geom *geom);

#ifdef __cpluscplus
}
#endif

#endif
