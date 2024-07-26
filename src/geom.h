/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
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

#include <stddef.h>
#include <stdbool.h>
#include "coordinatesequence.h"

/* --------------------------- Simple OGRGeometry --------------------------- */

/* point */
typedef struct geom_point_t {
    double x;
    double y;
    double z;
    double m;
} geom_point_t;

/* multi point */
typedef struct geom_mpoint_t {
    int     num;
    bool    hasZ;
    bool    hasM;
    double *coord;
} geom_mpoint_t;

/* linestring */
typedef struct geom_linestring_t {
    int     num;
    bool    hasZ;
    bool    hasM;
    double *coord;
} geom_linestring_t;

/* linearring */
typedef struct geom_linearring_t {
    int     num;
    bool    hasZ;
    bool    hasM;
    double *coord;
} geom_linearring_t;

/* multi linestring */
typedef struct geom_mlinestring_t {
    int                 num;
    bool                hasZ;
    bool                hasM;
    geom_linestring_t **geometries;
} geom_mlinestring_t;

/* polygon */
typedef struct geom_polygon_t {
    bool                hasZ;
    bool                hasM;
    geom_linearring_t  *shell;
    int                 hole_num;
    geom_linearring_t **holes;
} geom_polygon_t;

/* multi polygon */
typedef struct geom_mpolygon_t {
    bool             hasZ;
    bool             hasM;
    int              num;
    geom_polygon_t **geometries;
} geom_mpolygon_t;

/* -------------------------- Simple Math Geometry -------------------------- */

/* rectangle */
typedef struct geom_envelope_t {
    double minx;
    double maxx;
    double miny;
    double maxy;
} geom_envelope_t;

typedef struct geom_circle_t {
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};

/* ------------------------------ Geometry Base ----------------------------- */

/* coordinate */
struct SEGeom_t {
    int geom_id;
    int SRID;
    union {
        geom_point_t       *point;
        geom_linestring_t  *linestring;
        geom_linearring_t  *linearring;
        geom_polygon_t     *polygon;
        geom_mpoint_t      *mpoint;
        geom_mlinestring_t *mlinestring;
        geom_mpolygon_t    *mpolygon;
    } tio;
};


#endif