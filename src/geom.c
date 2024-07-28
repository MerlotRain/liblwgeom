#include "mg.h"
#include "geom.h"

struct tg_geom *geom_new_point(const struct tg_point p)
{
    return NULL;
}

struct tg_geom *geom_new_point_z(const struct tg_point p, double z)
{
    return NULL;
}

struct tg_geom *geom_new_point_m(const struct tg_point p, double m)
{
    return NULL;
}

struct tg_geom *geom_new_point_zm(const struct tg_point p, const double *zm)
{
    return NULL;
}

struct tg_geom *geom_new_point_empty()
{
    return NULL;
}

struct tg_geom *geom_new_path(const struct tg_path *path)
{
    return NULL;
}

struct tg_geom *geom_new_path_z(const struct tg_path *path,
                                const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_path_m(const struct tg_path *path,
                                const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_path_zm(const struct tg_path *path,
                                 const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_path_empty()
{
    return NULL;
}

struct tg_geom *geom_new_polygon(const struct tg_polygon *poly)
{
    return NULL;
}

struct tg_geom *geom_new_polygon_z(const struct tg_polygon *poly,
                                   const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_polygon_m(const struct tg_polygon *poly,
                                   const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_polygon_zm(const struct tg_polygon *poly,
                                    const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_polygon_empty()
{
    return NULL;
}

struct tg_geom *geom_new_mpoint(const struct tg_point *points, int npoints)
{
    return NULL;
}

struct tg_geom *geom_new_mpoint_z(const struct tg_point *points, int npoints,
                                  const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpoint_m(const struct tg_point *points, int npoints,
                                  const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpoint_zm(const struct tg_point *points, int npoints,
                                   const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpoint_empty()
{
    return NULL;
}

struct tg_geom *geom_new_mpath(const struct tg_path **paths, int nlines)
{
    return NULL;
}

struct tg_geom *geom_new_mpath_z(const struct tg_path **paths, int nlines,
                                 const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpath_m(const struct tg_path **paths, int nlines,
                                 const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpath_zm(const struct tg_path **paths, int nlines,
                                  const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpath_empty()
{
    return NULL;
}

struct tg_geom *geom_new_mpolygon(const struct tg_polygon **polys, int npolys)
{
    return NULL;
}

struct tg_geom *geom_new_mpolygon_z(const struct tg_polygon **polys, int npolys,
                                    const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpolygon_m(const struct tg_polygon **polys, int npolys,
                                    const double *extra_coords, int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpolygon_zm(const struct tg_polygon **polys,
                                     int npolys, const double *extra_coords,
                                     int ncoords)
{
    return NULL;
}

struct tg_geom *geom_new_mpolygon_empty()
{
    return NULL;
}

struct tg_geom *geom_new_geometrycollection(const struct tg_geom *const geoms[],
                                            int ngeoms)
{
    return NULL;
}

struct tg_geom *geom_new_geometrycollection_empty()
{
    return NULL;
}

void tg_geom_free(struct tg_geom *geom)
{
}