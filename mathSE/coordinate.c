
#include "coordinate.h"
#include "algorithm.h"
#include "factory.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>

// clang-format off

/**
 * @brief  sdo_geometry
 * 
 * | SDO_ETYPE    | SDO_INTERPRETATION | Meaning                                                                                
 * |--------------|--------------------|---------------------------------------------------------------------------------------|
 * | 0            | any numeric value  | Type 0 (zero) element. Used to model geometry                                         |
 * |              |                    | types not supported by Oracle Spatial and Graph.                                      |
 * | 1            | 1                  | Point type.                                                                           |
 * | 1            | n > 1              | Point cluster with n points.                                                          |
 * | 2            | 1                  | Line string whose vertices are connected by straight line segments.                   |
 * | 2            | 2                  | Line string made up of a connected sequence of circular arcs.                         |
 * |              |                    | Each circular arc is described using three coordinates:                               |
 * |              |                    | the start point of the arc, any point on the arc, and the end point of the arc.       |
 * |              |                    | The coordinates for a point designating the end of one arc and the start of           |
 * |              |                    | the next arc are not repeated. For example, five coordinates are used to              |
 * |              |                    | describe a line string made up of two connected circular arcs.                        |
 * |              |                    | Points 1, 2, and 3 define the first arc, and points 3, 4,                             |
 * |              |                    | and 5 define the second arc, where point 3 is only stored once.                       |
 * | 2            | 3                  | NURBS (non-uniform rational B-spline) curve.                                          |
 * | 1003 or 2003 | 1                  | Simple polygon whose vertices are connected by straight line segments.                |
 * |              |                    | You must specify a point for each vertex; and the last point specified must           |
 * |              |                    | be exactly the same point as the first (within the tolerance value),                  |
 * |              |                    | to close the polygon. For example, for a 4-sided polygon, specify 5 points,           |
 * |              |                    | with point 5 the same as point 1.                                                     |
 * | 1003 or 2003 | 2                  | Polygon made up of a connected sequence of circular arcs that closes on itself.       |
 * |              |                    | The end point of the last arc is the same as the start point of the first arc.        |
 * |              |                    | Each circular arc is described using three coordinates:                               |
 * |              |                    | the start point of the arc, any point on the arc, and the end point of the arc.       |
 * |              |                    | The coordinates for a point designating the end of one arc and the start of           |
 * |              |                    | the next arc are not repeated. For example, five coordinates are used to describe     |
 * |              |                    | a polygon made up of two connected circular arcs. Points 1, 2, and 3                  |
 * |              |                    | define the first arc, and points 3, 4, and 5 define the second arc.                   |
 * |              |                    | The coordinates for points 1 and 5 must be the same (tolerance is not considered),    |
 * |              |                    | and point 3 is not repeated.                                                          |
 * | 1003 or 2003 | 3                  | Rectangle type (sometimes called optimized rectangle). A bounding rectangle such      |
 * |              |                    | that only two points, the lower-left and the upper-right, are required to             |
 * |              |                    | describe it. The rectangle type can be used with geodetic or non-geodetic data.       |
 * |              |                    | However, with geodetic data, use this type only to create a                           |
 * |              |                    | query window (not for storing objects in the database).                               |
 * | 1003 or 2003 | 4                  | Circle type. Described by three distinct non-colinear points,                         |
 * |              |                    | all on the circumference of the circle.                                               |
 * | 4            | n > 1              | Compound line string with some vertices connected by straight line segments           |
 * |              |                    | and some by circular arcs. The value n in the Interpretation column                   |
 * |              |                    | specifies the number of contiguous subelements that make up the line string.          |
 * |              |                    | The next n triplets in the SDO_ELEM_INFO array describe each of these subelements.    |
 * |              |                    | The subelements can only be of SDO_ETYPE 2. The last point of a subelement is         |
 * |              |                    | the first point of the next subelement, and must not be repeated.                     |
 * | 1005 or 2005 | n > 1              | Compound polygon with some vertices connected by straight line segments and           |
 * |              |                    | some by circular arcs. The value n in the Interpretation column specifies             |
 * |              |                    | the number of contiguous subelements that make up the polygon.The next n              |
 * |              |                    | triplets in the SDO_ELEM_INFO array describe each of these subelements.               |
 * |              |                    | The subelements can only be of SDO_ETYPE 2. The end point of a subelement is          |
 * |              |                    | the start point of the next subelement, and it must not be repeated.                  |
 * |              |                    | The start and end points of the polygon must be exactly the same point                |
 * |              |                    | (tolerance is ignored).                                                               |
 *
*/

// clang-format on

double g_tolerance = 0.00001;

double tolerance(double tol) {
  double tmp = g_tolerance;
  g_tolerance = tol;
  return tmp;
}

/* ---------------------- coordinate factory functions ---------------------- */

coordinate *create_coordinate(uint32_t i_n, const int *i_p, uint32_t c_n,
                              uint32_t c_dim, const double *c_p, int flags) {
  return NULL;
}

void coordinate_info(coordinate *c, uint32_t *i_n, int **i_p, uint32_t *c_n,
                     uint32_t *c_dim, double **c_p) {}

void coordinate_destroy(coordinate *c) {}

coordinate *create_point(double *p) { return NULL; }

coordinate *create_line(double *p) { return NULL; }

coordinate *create_line2(double *p, double angle, double length) {
  return NULL;
}

coordinate *create_arc(double *p) { return NULL; }

coordinate *create_arc2(double *c, double radius, double angle1,
                        double angle2) {
  return NULL;
}

coordinate *create_arc3(double *p, double chord) { return NULL; }

coordinate *create_path(double *p, int num) { return NULL; }

coordinate *create_envelope(double *p) { return NULL; }

/* -------------------------- coordinate functions -------------------------- */

static int compare_double(double a, double b) {
  const double diff = a > b;
  return (int)(diff >= -4 * DBL_EPSILON) & (int)(diff <= 4 * DBL_EPSILON);
}

int compare_raw_point(struct raw_point *a, struct raw_point *b) {
  return compare_double(a->x, b->x) & compare_double(a->y, b->y);
}

int compare_raw_point3d(struct raw_point3d *a, struct raw_point3d *b) {

  return compare_double(a->x, b->x) & compare_double(a->y, b->y) &
         compare_double(a->z, b->z);
}
