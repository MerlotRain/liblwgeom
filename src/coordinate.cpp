#include "mathSE.h"

geom_coordinate_t *create_point(const double *p) {
    assert(p);
    geom_coordinate_t *point =
        (geom_coordinate_t *)malloc(sizeof(geom_coordinate_t));
    if (point == NULL)
        return NULL;

    point->oti.point = (geom_point_t *)malloc(sizeof(geom_point_t));
    if (!point->oti.point) {
        free(point);
        return NULL;
    }
    point->geom_id      = GEOMETRY_TYPE_POINT;
    point->oti.point->x = p[0];
    point->oti.point->y = p[1];
    return point;
}

geom_coordinate_t *create_arc1(const double *p) { return NULL; }

geom_coordinate_t *create_arc2(const double *c,
                                      const double  radius,
                                      const double  angle1,
                                      const double  angle2) { return NULL; }

geom_coordinate_t *create_arc3(const double *p, const double chord) { return NULL; }

geom_coordinate_t *create_linestring(const double *p, const int num) { return NULL; }

geom_coordinate_t *create_envelope(const double *p) { return NULL; }