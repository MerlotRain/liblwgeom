#ifndef __CRS_H__
#define __CRS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TRANS_WGS84_TO_CGCS2000 0
#define TRANS_WGS84_TO_XIAN80 1
#define TRANS_CGCS2000_TO_WGS84 3
#define TRANS_CGCS2000_TO_XIAN80 4
#define TRANS_XIAN80_TO_CGCS2000 5
#define TRANS_XIAN80_TO_WGS84 6

#define TRANS_FORWARD 1
#define TRANS_INVERSE -1

typedef struct __crs_transformation transformation;

transformation *create_trans(int epsg_from, int epsg_to);
transformation *create_wellknown_trans(int wellknown);
void destroy_trans(transformation *trans);

int trans_coord(transformation *trans, int direction, double *c);
int trans_coord_array(transformation *trans, int direction, double *c, int c_n);

#ifdef __cplusplus
}
#endif

#endif // __CRS_H__
