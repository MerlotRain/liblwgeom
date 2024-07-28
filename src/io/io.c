#include "factory.h"

#include "geom.h"

#include <stddef.h>

extern se_geom *geom_read_wkt(const char *data, int len);
extern se_geom *geom_read_wkb(const char *data, int len);
extern se_geom *geom_read_geojson(const char *data, int len);
extern se_geom *geom_read_ewkt(const char *data, int len);
extern se_geom *geom_read_ewkb(const char *data, int len);
extern se_geom *geom_read_kml(const char *data, int len);
extern se_geom *geom_read_gml(const char *data, int len);
extern se_geom *geom_read_gml2(const char *data, int len);

se_geom *geom_read(const char *data, int len, int flag)
{
    if (flag == GEOMETRY_IO_TYPE_WKT) {
        return geom_read_wkt(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_WKB) {
        return geom_read_wkb(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_GEOJSON) {
        return geom_read_geojson(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_EWKT) {
        return geom_read_ewkt(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_EWKB) {
        return geom_read_ewkb(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_KML) {
        return geom_read_kml(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_GML) {
        return geom_read_gml(data, len);
    }
    else if (flag == GEOMETRY_IO_TYPE_GML2) {
        return geom_read_gml2(data, len);
    }
    return NULL;
}

se_geom *geom_read_ora(int i_n, const int *i_p, int c_n, int c_dim,
                       const double *c_p, int flag)
{
}

void geome_write(const se_geom *geom, char **data, int *len, int flag)
{
}

void geom_write_ora(const se_geom *geom, int *i_n, int **i_p, int *c_n,
                    double **c_p)
{
}

void geom_free(se_geom *geom)
{
}
