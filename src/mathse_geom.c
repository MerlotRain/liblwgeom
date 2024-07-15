#include "mathse_geom.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* ----------------------------- Geometry Utils ----------------------------- */
bool segment_arc(const geom_point_t p1,
                 const geom_point_t p2,
                 const geom_point_t p3,
                 geom_point_t     **points,
                 int               *num);




/* ---------------------------- Geometry Convert ---------------------------- */

geom_coordinate_t *create_point(const double *p) {
    assert(p);
    geom_coordinate_t *coord =
        (geom_coordinate_t *)malloc(sizeof(geom_coordinate_t));
    if (coord == NULL)
        return NULL;

    coord->oti.point = (geom_point_t *)malloc(sizeof(geom_point_t));
    if (!coord->oti.point) {
        free(coord);
        return NULL;
    }
    coord->geom_id      = GEOMETRY_TYPE_POINT;
    coord->oti.point->x = p[0];
    coord->oti.point->y = p[1];
    return coord;
}

geom_coordinate_t *convert_line_to_coordinate(const geom_line_t *p)
{
    assert(p);
    geom_coordinate_t *coord =
        (geom_coordinate_t *)malloc(sizeof(geom_coordinate_t));
    if (coord == NULL)
        return NULL;

    coord->oti.linestring =
        (geom_linestring_t *)malloc(sizeof(geom_linestring_t));
    if (!coord->oti.linearring) {
        free(coord);
        return NULL;
    }

    coord->geom_id               = GEOMETRY_TYPE_LINEARRING;
    coord->oti.linestring->num   = 2;
    coord->oti.linestring->coord = (double *)malloc(sizeof(double) * 4);
    memcpy(coord->oti.linestring->coord, p, sizeof(double) * 4);
    return coord;
}

geom_coordinate_t *convert_arc_to_coordinate(const geom_arc_t *p)
{
    assert(p);
    geom_point_t* points;
    int point_size = 0;
    if(!segment_arc())
    {
        fprinf(stderr, "segment arc to line error");
        return NULL;
    }

    geom_coordinate_t *coord =
        (geom_coordinate_t *)malloc(sizeof(geom_coordinate_t));
    if (coord == NULL)
        return NULL;

    coord->oti.linestring =
        (geom_linestring_t *)malloc(sizeof(geom_linestring_t));
    if (!coord->oti.linearring) {
        free(coord);
        return NULL;
    } 

    coord->geom_id               = GEOMETRY_TYPE_LINEARRING;
    coord->oti.linestring->num   = point_size;
    coord->oti.linestring->coord = points;
    return coord;
}

geom_coordinate_t *convert_envelope_to_coordinate(const geom_envelope_t* p)
{
    return NULL;
}

geom_coordinate_t *convert_ellipse_to_coordinate(const geom_ellipse_t* p, bool fill)
{
    return NULL;
}

geom_coordinate_t *convert_triangle_to_coordinate(const geom_triangle_t* p)
{
    return NULL;
}