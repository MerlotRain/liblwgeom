#ifndef __MATHSE_ALGORITHM_H__
#define __MATHSE_ALGORITHM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct geom_coordinate_t geom_coordinate_t;
typedef struct se_i4_t se_i4_t;

/* Calculate the length of a geometry */
#define GEOMETRY_PROP_VALUE_LENGTH 0
/* Calculate the width of a geometry */
#define GEOMETRY_PROP_VALUE_WIDTH 1
/* Calculate the height of a geometry */
#define GEOMETRY_PROP_VALUE_HEIGHT 2
/* Calculate the area of a geometry */
#define GEOMETRY_PROP_VALUE_AREA 3

/* Get a clone of the geometry */
#define GEOMETRY_PROP_GEO_CLONE 10
/* Get the marker point of the geometry, which must be inside the geometry */
#define GEOMETRY_PROP_GEO_LABEL 11
/* Get the center point of the geometry */
#define GEOMETRY_PROP_GEO_CENTER 12
/* Get the centroid point of the geometry */
#define GEOMETRY_PROP_GEO_CENTROID 13
/* Get the envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE 14
/* Get the oriented minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ORIENTED_ENVELOPE 15
/* Get the minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE 16
/* Get the inner minimum rectangle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_RECT 17
/* Get the inner minimum square of the geometry */
#define GEOMETRY_PROP_GEO_INNER_SQUARE 18
/* Get the inner minimum circle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_CIRCLE 19
/* Get the smallest convex polygon that contains all the points int the geometry
 */
#define GEOMETRY_PROP_GEO_CONVEX_HULL 20
/* Get the simplified geometry */
#define GEOMETRY_PROP_GEO_SIMPLIFY 21
/* Geometric simplification that preserves the original geometric topology */
#define GEOMETRY_PROP_GEO_TOPOLOGY_PRESERVING_SIMPLIFY 22
/* Get the boundary of the geometry */
#define GEOMETRY_PROP_GEO_BOUNDARY 23

/* Delete all duplicate points, and pass in the duplicate point judgment
 * threshold (double) in para */
#define GEOMETRY_MODIFY_REMOVE_REPEAT 40
/* Douglas compression algorithm, para is the compression algorithm threshold
 * (double) */
#define GEOMETRY_MODIFY_DOUGLAS 41
/* Reverse the order of the points in the geometry, para is null */
#define GEOMETRY_MODIFY_REVERSE 42

/* Computes the result of union two geometries */
#define GEOMETRY_COMBINE_UNION 50
/* Computes the result of the intersection of two geometries */
#define GEOMETRY_COMBINE_INTERSECT 51
/* Computes the result of the difference of two geometries*/
#define GEOMETRY_COMBINE_DIFFERENCE 52
/* Calculate the result of the difference between two geometric symmetries */
#define GEOMETRY_COMBINE_SYM_DIFFERENCE 53

/* Two geometries do not fit this relationship */
#define GEOMETRY_RELATION_RESULT_UNFIT -1
/* The two geometries do not satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_FALSE 0
/* Two geometries satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_TRUE 1

/* Two geometries is disjoint each other */
#define GEOMETRY_SPR_IS_DISJOINT 60
/* Two geometries is intersect each other */
#define GEOMETRY_SPR_IS_INTERSECT 61
/* Two geometries is contain each other */
#define GEOMETRY_SPR_IS_CONTAIN 62
/* Two geometries is cross each other */
#define GEOMETRY_SPR_IS_CROSS 63
/* Two geometries is equal */
#define GEOMETRY_SPR_IS_EQUAL 64
/* Two geometries is touch each other */
#define GEOMETRY_SPR_IS_TOUCH 65
/* Two geometries is overlap each other */
#define GEOMETRY_SPR_IS_OVERLAP 66
/* Two geometries is within each other */
#define GEOMETRY_SPR_IS_WITHIN 67

/* Check geometry is simple */
#define GEOMETRY_CHECK_1_SIMPLE 80
/* Check geometry is close */
#define GEOMETRY_CHECK_1_CLOSE 81
/* Check geometry is good direction */
#define GEOMETRY_CHECK_1_GOOD_DIRECTION 82
/* Check geometry is self cross */
#define GEOMETRY_CHECK_1_SELF_CROSS 83
/* Check geometry has repeat point */
#define GEOMETRY_CHECK_1_REPEAT 84
/* Check line is lap */
#define GEOMETRY_CHECK_1_LAP 85

/* Check geometry repeat points */
#define GEOMETRY_CHECK_2_REPEAT_POINT 0x01
/* Check geometry isolate points */
#define GEOMETRY_CHECK_2_ISOLATE_POINT 0x02
/* Check geometry pseudo endpoints */
#define GEOMETRY_CHECK_2_PSEUDO_ENDPOINT 0x04

/* Set global tolerance
 * tol: new tolerance
 * return: old tolerance */
SE_API double tolerance(double tol);

/* Compute numerical properties of geometry */
SE_API double geometry_prop_value(const geom_coordinate_t *c, int mode);

/* Get the geometric properties of a geometry */
SE_API geom_coordinate_t *geometry_prop_geo(const geom_coordinate_t *c,
                                            int                      mode);

/* Obtain simple geometric properties of the geometry. These properties usually
 * only require a fixed number of point coordinates to describe, such as center,
 * center of gravity, label point, circumscribed rectangle, inscribed circle,
 * circumscribed circle, inscribed circle*/
SE_API int
geometry_prop_geo2(const geom_coordinate_t *c, int mode, double **result);

/* Modify geometry properties */
SE_API int geometry_modify(geom_coordinate_t *c, int mode, void *para);
/* Computes the result of two geometric combination operations */
SE_API geom_coordinate_t *geometry_combine(const geom_coordinate_t *a,
                                           const geom_coordinate_t *b,
                                           int                      mode);

/* Query whether two geometric relationships satisfy the specified spatial
 * relationship */
SE_API int geometry_spr_query_is(const geom_coordinate_t *a,
                                 const geom_coordinate_t *b,
                                 int                      mode);
/* Determine the spatial relationship between two geometries and obtain the
 * intersection matrix, The matrix is an integer array of length 9 */
SE_API int geometry_spr_query(const geom_coordinate_t *a,
                              const geom_coordinate_t *b,
                              int                     *matrix);

/* Check individual geometric correctness */
SE_API int geometry_check_1(const geom_coordinate_t *c, int mode);

/* Checks a set of geometries for correctness and writes error messages to the
 * stream */
SE_API int geometry_check_2(
    const se_i4_t *r, const int r_n, se_i4_t **w, int *w_n, int mode);

#ifdef __cplusplus
}
#endif

#endif//__MATHSE_ALGORITHM_H__