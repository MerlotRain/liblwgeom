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
 * |              |                    | Each circular arc is described using three coordinate_blob_ts:                        |
 * |              |                    | the start point of the arc, any point on the arc, and the end point of the arc.       |
 * |              |                    | The coordinate_blob_ts for a point designating the end of one arc and the start of    |
 * |              |                    | the next arc are not repeated. For example, five coordinate_blob_ts are used to       |
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
 * |              |                    | Each circular arc is described using three coordinate_blob_ts:                        |
 * |              |                    | the start point of the arc, any point on the arc, and the end point of the arc.       |
 * |              |                    | The coordinate_blob_ts for a point designating the end of one arc and the start of    |
 * |              |                    | the next arc are not repeated. For example, five coordinate_blob_ts are used to       |
 * |              |                    | describe                                                                              |
 * |              |                    | a polygon made up of two connected circular arcs. Points 1, 2, and 3                  |
 * |              |                    | define the first arc, and points 3, 4, and 5 define the second arc.                   |
 * |              |                    | The coordinate_blob_ts for points 1 and 5 must be the same (tolerance is not          |
 * |              |                    | considered),                                                                          |
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
