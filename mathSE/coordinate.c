#include "mathSE.h"

geom_coordinate_t *create_point(const double *p) {
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

geom_coordinate_t *create_line1(const double *p) {
    geom_coordinate_t *line =
        (geom_coordinate_t *)malloc(sizeof(geom_coordinate_t));
    if (line == NULL)
        return NULL;

    line->oti.line = (geom_line_t *)malloc(sizeof(geom_line_t));
}
