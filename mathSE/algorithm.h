#ifndef ALGORITHM_H
#define ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

// Invalid geometry type
#define GEOMETRY_TYPE_NONE 0
// single point geometry
#define GEOMETRY_TYPE_POINT 1
// Path usually consists of one or a group of line segments connected end to
// end. It can be a straight line segment, an arc segment or a bezier curve.
#define GEOMETRY_TYPE_PATH 2
// Polygon is usually composed of a group of closed Rings, and there is at least
// one outer ring in these Rings. If there is only one outer ring, then the
// other rings are inner rings of the outer ring.
#define GEOMETRY_TYPE_POLYGON 3
// Collection is the parent type of Polygon and Polyline, but we usually use
// collection to represent it. The geometric types of elements in collection are
// inconsistent. If the geometry type is the same, it is usually represented by
// polygon or polyline.
#define GEOMETRY_TYPE_COLLECTION 4
#define GEOMETRY_TYPE_MULTIPOINT 5 // 复合点、多点
#define GEOMETRY_TYPE_POLYLINE 6   // 多段折线
#define GEOMETRY_TYPE_ENVELOPE 7   // 矩形
#define GEOMETRY_TYPE_LINE 8       // 两点线段
#define GEOMETRY_TYPE_CIRCLEARC 9  // 三点圆弧
#define GEOMETRY_TYPE_RING 10      // 单圈

typedef struct coordinate_blob coordinate;

coordinate *create_coordinate(int i_n, const int *i_p, int c_n, int c_dim,
                              const double *c_p, int flags);
void coordinate_info(coordinate *c, int *i_n, int **i_p, int *c_n, int *c_dim,
                     double **c_p);
void coordinate_destroy(coordinate *c);

double tolerance(double tol);

/* -------------------------------------------------------------------------- */
/*                            coordinate attribute                            */
/* -------------------------------------------------------------------------- */
int geometry_type(coordinate *c);
int *coordinate_interpret(coordinate *c);
int coordinate_interpret_n(coordinate *c);
double *coordinate_coord(coordinate *c);
unsigned int coordinate_point_n(coordinate *c);
int coordinate_dim_c(coordinate *c);
int coordinate_dim_g(coordinate *c);
int coordinate_sub_n(coordinate *c);
coordinate *coordinate_sub_at(coordinate *c, int index);

/* -------------------------------------------------------------------------- */
/*                          coordinate value property                         */
/* -------------------------------------------------------------------------- */
double value_area(coordinate *c);
double value_length(coordinate *c);
double value_width(coordinate *c);
double value_height(coordinate *c);

/* -------------------------------------------------------------------------- */
/*                        coordinate geometry property                        */
/* -------------------------------------------------------------------------- */
coordinate *geometry_clone(coordinate *c);
coordinate *geometry_label(coordinate *c);
coordinate *geometry_center(coordinate *c);
coordinate *geometry_centroid(coordinate *c);
coordinate *geometry_envelope(coordinate *c);

void geometry_label2(coordinate *c, double *result);
void geometry_center2(coordinate *c, double *result);
void geometry_centroid2(coordinate *c, double *result);
void geometry_envelope2(coordinate *c, double *result);

#ifdef __cplusplus
}
#endif

#endif // ALGORITHM_H