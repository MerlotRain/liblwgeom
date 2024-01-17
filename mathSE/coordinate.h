#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 sdo_geometry 
 https://docs.oracle.com/cd/A97630_01/appdev.920/a96630/sdo_objrelschema.htm 
 
 | SDO_ETYPE    | SDO_INTERPRETATION | Meaning                                   
 |--------------|--------------------|-------------------------------------------|
 | 0            | any numeric value  | Type 0 (zero) element. Used to model      |
 |              |                    | geometry types not supported by Oracle    |
 |              |                    | Spatial and Graph.                        |
 |--------------|--------------------|-------------------------------------------|
 | 1            | 1                  | Point type.                               |
 |--------------|--------------------|-------------------------------------------|
 | 1            | n > 1              | Point cluster with n points.              |
 | 2            | 1                  | Line string whose vertices are connected  |
 |              |                    | by straight line segments.                |
 |--------------|--------------------|-------------------------------------------|
 | 2            | 2                  | Line string made up of a connected        |
 |              |                    | sequence of circular arcs.                |
 |              |                    | Each circular arc is described using 3    |
 |              |                    | coordinate_blob_ts:                       |
 |              |                    | the start point of the arc, any point on  |
 |              |                    | the arc, and the end point of the arc.    |
 |              |                    | The coordinate_blob_ts for a point        |
 |              |                    | designating the end of one arc and the    |
 |              |                    | start of the next arc are not repeated.   |
 |              |                    | For example, five coordinate_blob_ts are  |
 |              |                    | used to describe a line string made up of |
 |              |                    | two connected circular arcs.              |
 |              |                    | Points 1, 2, and 3 define the first arc,  |
 |              |                    | and points 3, 4, and 5 define the second  |
 |              |                    | arc, where point 3 is only stored once.   |
 |--------------|--------------------|-------------------------------------------|
 | 2            | 3                  | NURBS (non-uniform rational B-spline)     |
 |              |                    | curve.                                    |
 |--------------|--------------------|-------------------------------------------|
 | 1003 or 2003 | 1                  | Simple polygon whose vertices are         |
 |              |                    | connected by straight line segments.      |
 |              |                    | You must specify a point for each vertex; |
 |              |                    | and the last point specified must be      |
 |              |                    | exactly the same point as the first       |
 |              |                    | (within the tolerance value), to close the|
 |              |                    | polygon. For example, for a 4-sided       |
 |              |                    | polygon, specify 5 points, with point 5   |
 |              |                    | the same as point 1.                      |
 |--------------|--------------------|-------------------------------------------|
 | 1003 or 2003 | 2                  | Polygon made up of a connected sequence of|
 |              |                    | circular arcs that closes on itself.      |
 |              |                    | The end point of the last arc is the same |
 |              |                    | as the start point of the first arc.      |
 |              |                    | Each circular arc is described using three|
 |              |                    | coordinate_blob_ts:                       |
 |              |                    | the start point of the arc, any point on  |
 |              |                    | the arc, and the end point of the arc.    |
 |              |                    | The coordinate_blob_ts for a point        |
 |              |                    | designating the end of one arc and the    |
 |              |                    | start of the next arc are not repeated.   |
 |              |                    | For example, five coordinate_blob_ts are  |
 |              |                    | used to describe a polygon made up of two |
 |              |                    | connected circular arcs. Points 1, 2, and |
 |              |                    | 3 define the first arc, and points 3, 4,  |
 |              |                    | and 5 define the second arc.              |
 |              |                    | The coordinate_blob_ts for points 1 and 5 |
 |              |                    | must be the same (tolerance is not        |
 |              |                    | considered), and point 3 is not repeated. |
 |--------------|--------------------|-------------------------------------------|
 | 1003 or 2003 | 3                  | Rectangle type (sometimes called optimized|
 |              |                    | rectangle). A bounding rectangle such that|
 |              |                    | only two points, the lower-left and the   |
 |              |                    | upper-right, are required to describe it. |
 |              |                    | The rectangle type can be used with       |
 |              |                    | geodetic or non-geodetic data.            |
 |              |                    | However, with geodetic data, use this type|
 |              |                    | only to create a query window (not for    |
 |              |                    | storing objects in the database).         |
 |--------------|--------------------|-------------------------------------------|
 | 1003 or 2003 | 4                  | Circle type. Described by three distinct  |
 |              |                    | coordinate_blob_ts: non-colinear points,  |
 |              |                    | all on the circumference of the circle.   |
 |--------------|--------------------|-------------------------------------------|
 | 4            | n > 1              | Compound line string with some vertices   |
 |              |                    | connected by straight line segments and   |
 |              |                    | some by circular arcs. The value n in the |
 |              |                    | Interpretation column specifies the number|
 |              |                    | of contiguous subelements that make up the|
 |              |                    | line string.                              |
 |              |                    | The next n triplets in the SDO_ELEM_INFO  |
 |              |                    | array describe each of these subelements. |
 |              |                    | The subelements can only be of SDO_ETYPE 2| 
 |              |                    | The last point of a subelement is the     |
 |              |                    | first point of the next subelement,       |
 |              |                    | and must not be repeated.                 |
 |--------------|--------------------|-------------------------------------------|
 | 1005 or 2005 | n > 1              | Compound polygon with some vertices       |
 |              |                    | connected by straight line segments and   |
 |              |                    | some by circular arcs. The value n in the |
 |              |                    | Interpretation column specifies the number|
 |              |                    | of contiguous subelements that make up the|
 |              |                    | polygon.The next n triplets in the        |
 |              |                    | SDO_ELEM_INFO array describe each of these|
 |              |                    | subelements.                              |
 |              |                    | The subelements can only be of SDO_ETYPE  |
 |              |                    | 2. The end point of a subelement is the   |
 |              |                    | start point of the next subelement, and it|
 |              |                    | must not be repeated.                     |
 |              |                    | The start and end points of the polygon   |
 |              |                    | must be exactly the same point (tolerance |
 |              |                    | is ignored).                              |
 |--------------|--------------------|-------------------------------------------|
 
 */

/* The geometry element type is point. */
#define INTERPRET_TYPE_POINT 1
/* The geometry element type is line. */
#define INTERPRET_TYPE_LINE 2
/* The geometry element type is an exterior ring of polygon. */
#define INTERPRET_TYPE_POLYGON_EXTERIOR 1003
/* The geometry element type is an interior ring of polygon. */
#define INTERPRET_TYPE_POLYGON_INTERIOR 2003
/* The geometry element type is a compound line. */
#define INTERPRET_TYPE_COMPOUND_LINE 4
/* The geometry element type is an exterior ring of compound polygon. */
#define INTERPRET_TYPE_COMPOUND_POLYGON_EXTERIOR 1005
/* The geometry element type is an interior ring of compound polygon. */
#define INTERPRET_TYPE_COMPOUND_POLYGON_INTERIOR 2005

/* The geometry interpretation is single point. */
#define INTERPRET_PRE_SINGLE_POINT 1
/* The geometry interpretation is segment line. */
#define INTERPRET_PRE_LINE_PATH 1
/* The geometry interpretation is arc line. */
#define INTERPRET_PRE_LINE_ARC 2
/* The geometry interpretation is bezier line. */
#define INTERPRET_PRE_LINE_BEZIER 3
/* The geometry interpretation is path polygon. */
#define INTERPRET_PRE_POLYGON_PATH 1
/* The geometry interpretation is arc polygon. */
#define INTERPRET_PRE_POLYGON_ARC 2
/* The geometry interpretation is rectangle. */
#define INTERPRET_PRE_POLYGON_RECTANGLE 3
/* The geometry interpretation is circle. */
#define INTERPRET_PRE_POLYGON_CIRCLE 4

/* The geometry memory model is a simplified SDO_Geometry model.
 * geo_type: geometry type
 * interpret_num: interpret number
 * coord_num: coordinate number
 * ref: ref: default value is 0. 1: copy the coordinate memory. 0: ref input
 coordinate memory
 * interpret: interpret pointer, it's range is [interpret, interpret + 3 *
 interpret_num)
 * coord: coordinate pointer, it's range is [coord, coord + coord_num)
 */
typedef struct se_coordinate_blob {
  uint8_t geo_type;
  uint32_t interpret_num;
  uint32_t coord_num;
  uint8_t ref;
  int *interpret;
  double *coord;
} coordinate_blob_t;

typedef struct se_interpret_tuple {
  int Offset; // double offset, begin 1
  int Type;   // element type
  int Pre;    // interpretation
} interpret_tuple_t;

#define INIT_INTERPRET_POINT                                                   \
  {.Offset = 1, .Type = INTERPRET_TYPE_POINT, .Pre = 1};

#define INIT_INTERPRET_MULTI_POINT(n)                                          \
  {.Offset = 1, .Type = INTERPRET_TYPE_POINT, .Pre = n};

#define INIT_INTERPRET_LINE_PATH(offset)                                       \
  {.Offset = offset,                                                           \
   .Type = INTERPRET_TYPE_LINE,                                                \
   .Pre = INTERPRET_PRE_LINE_PATH};

#define INIT_INTERPRET_LINE_ARC(offset)                                        \
  {.Offset = offset,                                                           \
   .Type = INTERPRET_TYPE_LINE,                                                \
   .Pre = INTERPRET_PRE_LINE_ARC};

#define INIT_INTERPRET_LINE_B_SHAPE(offset)                                    \
  {.Offset = offset,                                                           \
   .Type = INTERPRET_TYPE_LINE,                                                \
   .Pre = INTERPRET_PRE_LINE_B_SHAPE};

#define INIT_INTERPRET_EXTERIOR_RING(offset)                                   \
  {.Offset = offset,                                                           \
   .Type = INTERPRET_TYPE_POLYGON_EXTERIOR,                                    \
   .Pre = INTERPRET_PRE_POLYGON_PATH};

#define INIT_INTERPRET_INTERIOR_RING(offset)                                   \
  {.Offset = offset,                                                           \
   .Type = INTERPRET_TYPE_POLYGON_INTERIOR,                                    \
   .Pre = INTERPRET_PRE_POLYGON_PATH};

#ifdef __cplusplus
}
#endif

#endif // __COORDINATE_H__