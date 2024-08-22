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
#include "mghelp.h"
#include "rtree.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief tolerance value
extern double tolerence();

/// @brief tolerance value macro
#define MG_TOLERANCE tolerence()

/// @brief calculate point distance
#define MG_POINTDISTANCE(x0, y0, x1, y1) \
    sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define MG_POINTDISTANCE2(A, B) MG_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)

/// @brief check point A, B is equal
#define MG_POINT_EQUAL(A, B)    (MG_POINTDISTANCE2((A), (B)) < MG_TOLERANCE)

/// @brief return point A angle
#define MG_POINT_ANGLE(A)       (atan2((A).y, (A).x))

/// @brief check double value is equal
#define MG_DOUBLE_NEARES(A)     (fabs((A)) < MG_TOLERANCE)

/// @brief check double value is equal
#define MG_DOUBLE_NEARES2(A, B) (fabs((A) - (B)) < MG_TOLERANCE)

/* ---------------------------- mg kernel struct ---------------------------- */

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
    size_t size;         ///< size of object array
    struct mg_i4 **objs; ///< object array
    struct rtree *index; ///< spatial index
};

/* ----------------------- geometry IO extern function ---------------------- */

struct mg_object *mg_read_wkt(const char *wkt, size_t len);
struct mg_object *mg_read_wkb(const char *wkb, size_t len, bool hex);
struct mg_object *mg_read_ewkt(const char *ewkt, size_t len);
struct mg_object *mg_read_ewkb(const char *ewkb, size_t len);
struct mg_object *mg_read_geojson(const char *json, size_t len);
struct mg_object *mg_read_kml(const char *kml, size_t len);
struct mg_object *mg_read_gml2(const char *gml, size_t len);
struct mg_object *mg_read_gml3(const char *gml, size_t len);

int mg_write_wkt(const struct mg_object *obj, char **wkt, size_t *len);
int mg_write_wkb(const struct mg_object *obj, bool hex, char **wkb,
                 size_t *len);
int mg_write_ewkt(const struct mg_object *obj, char **ewkt, size_t *len);
int mg_write_ewkb(const struct mg_object *obj, char **ewkb, size_t *len);
int mg_write_geojson(const struct mg_object *obj, char **json, size_t *len);
int mg_write_kml(const struct mg_object *obj, char **kml, size_t *len);
int mg_write_gml2(const struct mg_object *obj, char **gml, size_t *len);
int mg_write_gml3(const struct mg_object *obj, char **gml, size_t *len);

/* ------------------- geometry algorithm extern function ------------------- */

bool mg_check_single_ring(const double *pp, int npoints, int cdim);

double mg_prop_length_value(const struct mg_object *obj);
double mg_prop_area_value(const struct mg_object *obj);
double mg_prop_width_value(const struct mg_object *obj);
double mg_prop_height_value(const struct mg_object *obj);

struct mg_object *mg_prop_geo_clone(const struct mg_object *obj);
void mg_prop_geo_centriod(const struct mg_object *obj, double *xy);

struct mg_object *
mg_prop_geo_simpily_douglaspeucker(const struct mg_object *obj);

#ifdef __cplusplus
}
#endif

#endif