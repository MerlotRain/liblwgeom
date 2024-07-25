#ifndef __MATHSE_CRS_H__
#define __MATHSE_CRS_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Conversion between WGS84 coordinate system and CGCS2000 coordinate system,
 * the default is from WGS84 to CGCS2000 */
#define TRANS_WGS84_WITH_CGCS2000
/* Conversion between WGS84 coordinate system and XIAN80 coordinate system,
 * the default is from WGS84 to XIAN80 */
#define TRANS_WGS84_WITH_XIAN80
/* Conversion between CGCS2000 coordinate system and XIAN80 coordinate system,
 * the default is from CGCS2000 to XIAN80 */
#define TRANS_CGCS2000_WITH_XIAN80

/* Apply transformation to observation - in forward or inverse direction */
/* Forward */
#define TRANS_FORWARD 1
/* do nothing */
#define TRANS_NONE 0
/* Inverse */
#define TRANS_INVERSE -1

/* Spatial coordinate conversion */
typedef struct se_crs_transform_t crs_transformation_t;

/* Creates a coordinate transformation based on the EPSG value, or returns NULL
 * if the EPSG value is not found
 * epsg_from: from epsg code epsg_to: to epsg
 * code return: transformation object */
SE_API crs_transformation_t *create_trans(int epsg_from, int epsg_to);
/* Create commonly used coordinate transformations */
SE_API crs_transformation_t *create_wellknown_trans(int wellknown);
void                         destroy_trans(crs_transformation_t *trans);

/* trans point */
SE_API int
trans_coord(const crs_transformation_t *trans, int direction, double *c);
/* trans point array */
SE_API int trans_coord_array(const crs_transformation_t *trans,
                             int                         direction,
                             double                     *c,
                             int                         c_n);


#ifdef __cplusplus
}
#endif

#endif//__MATHSE_CRS_H__