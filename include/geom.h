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

#ifndef GEOM_H
#define GEOM_H

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

/// The base point type used for all geometries.
struct tg_point {
    double x;
    double y;
};

struct tg_segment {
    struct tg_point p1;
    struct tg_point p2;
};

struct tg_envelope {
    struct tg_point min;
    struct tg_point max;
};

struct tg_path;
struct tg_ring;
struct tg_polygon;
struct tg_geom;

enum geom_type {
    MG_POINT = 1,              ///< Point
    MG_PATH = 2,               ///< Path
    MG_RING = 3,               ///< Ring, closed path
    MG_POLYGON = 4,            ///< Polygon
    MG_MULTIPOINT = 5,         ///< MultiPoint, collection of points
    MG_MULTILINESTRING = 6,    ///< MultiLinestring, collection of paths
    MG_MULTIPOLYGON = 7,       ///< MultiPolygon, collection of polygons
    MG_GEOMETRYCOLLECTION = 8, ///< GeometryCollection, collection of geometries
};

// clang-format on

EXTERN struct tg_geom *geom_new_point(struct tg_point point);
EXTERN struct tg_geom *geom_new_point_z(struct tg_point point, double z);
EXTERN struct tg_geom *geom_new_point_m(struct tg_point point, double m);
EXTERN struct tg_geom *geom_new_point_zm(struct tg_point point,
                                         const double *zm);
EXTERN struct tg_geom *geom_new_point_empty();

EXTERN struct tg_geom *geom_new_path(const struct tg_path *path);
EXTERN struct tg_geom *geom_new_path_z(const struct tg_path *path,
                                       const double *extra_coords, int ncoords);
EXTERN struct tg_geom *geom_new_path_m(const struct tg_path *path,
                                       const double *extra_coords, int ncoords);
EXTERN struct tg_geom *geom_new_path_zm(const struct tg_path *path,
                                        const double *extra_coords,
                                        int ncoords);
EXTERN struct tg_geom *geom_new_path_empty();

EXTERN struct tg_geom *geom_new_polygon(const struct tg_polygon *poly);
EXTERN struct tg_geom *geom_new_polygon_z(const struct tg_polygon *poly,
                                          const double *extra_coords,
                                          int ncoords);
EXTERN struct tg_geom *geom_new_polygon_m(const struct tg_polygon *poly,
                                          const double *extra_coords,
                                          int ncoords);
EXTERN struct tg_geom *geom_new_polygon_zm(const struct tg_polygon *poly,
                                           const double *extra_coords,
                                           int ncoords);
EXTERN struct tg_geom *geom_new_polygon_empty();

EXTERN struct tg_geom *geom_new_mpoint(const struct tg_point *points,
                                       int npoints);
EXTERN struct tg_geom *geom_new_mpoint_z(const struct tg_point *points,
                                         int npoints,
                                         const double *extra_coords,
                                         int ncoords);
EXTERN struct tg_geom *geom_new_mpoint_m(const struct tg_point *points,
                                         int npoints,
                                         const double *extra_coords,
                                         int ncoords);
EXTERN struct tg_geom *geom_new_mpoint_zm(const struct tg_point *points,
                                          int npoints,
                                          const double *extra_coords,
                                          int ncoords);
EXTERN struct tg_geom *geom_new_mpoint_empty();

EXTERN struct tg_geom *geom_new_mpath(const struct tg_path **paths, int nlines);
EXTERN struct tg_geom *geom_new_mpath_z(const struct tg_path **paths,
                                        int nlines, const double *extra_coords,
                                        int ncoords);
EXTERN struct tg_geom *geom_new_mpath_m(const struct tg_path **paths,
                                        int nlines, const double *extra_coords,
                                        int ncoords);
EXTERN struct tg_geom *geom_new_mpath_zm(const struct tg_path **paths,
                                         int nlines, const double *extra_coords,
                                         int ncoords);
EXTERN struct tg_geom *geom_new_mpath_empty();

EXTERN struct tg_geom *geom_new_mpolygon(const struct tg_polygon **polys,
                                         int npolys);
EXTERN struct tg_geom *geom_new_mpolygon_z(const struct tg_polygon **polys,
                                           int npolys,
                                           const double *extra_coords,
                                           int ncoords);
EXTERN struct tg_geom *geom_new_mpolygon_m(const struct tg_polygon **polys,
                                           int npolys,
                                           const double *extra_coords,
                                           int ncoords);
EXTERN struct tg_geom *geom_new_mpolygon_zm(const struct tg_polygon **polys,
                                            int npolys,
                                            const double *extra_coords,
                                            int ncoords);
EXTERN struct tg_geom *geom_new_mpolygon_empty();

EXTERN struct tg_geom *
geom_new_geometrycollection(const struct tg_geom *const geoms[], int ngeoms);
EXTERN struct tg_geom *geom_new_geometrycollection_empty();

EXTERN void geom_free(struct tg_geom *geom);

// clang-format on

EXTERN struct tg_geom *geom_read_wkt(const char *data, int len);
EXTERN struct tg_geom *geom_read_wkb(const char *data, int len);
EXTERN struct tg_geom *geom_read_geojson(const char *data, int len);
EXTERN struct tg_geom *geom_read_ewkt(const char *data, int len);
EXTERN struct tg_geom *geom_read_ewkb(const char *data, int len);
EXTERN struct tg_geom *geom_read_kml(const char *data, int len);
EXTERN struct tg_geom *geom_read_gml(const char *data, int len);
EXTERN struct tg_geom *geom_read_gml2(const char *data, int len);

EXTERN int geom_write_wkt(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_wkb(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_geojson(const struct tg_geom *g, char *data, int len);
EXTERN int geom_write_ewkt(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_ewkb(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_kml(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_gml(const struct tg_geom *g, char **data, int len);
EXTERN int geom_write_gml2(const struct tg_geom *g, char **data, int len);

#ifdef __cplusplus
}
#endif

#endif