#ifndef ALGORITHM_H
#define ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

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
double coordinate_value_property(coordinate *c, int property);
double value_area(coordinate *c);
double value_length(coordinate *c);
double value_width(coordinate *c);
double value_height(coordinate *c);

/* -------------------------------------------------------------------------- */
/*                        coordinate geometry property                        */
/* -------------------------------------------------------------------------- */
coordinate *coordinate_geometry_property(coordinate *c, int property);
coordinate *geometry_clone(coordinate *c);
coordinate *geometry_label(coordinate *c);
coordinate *geometry_center(coordinate *c);
coordinate *geometry_centroid(coordinate *c);
coordinate *geometry_envelope(coordinate *c);
void coordinate_geometry_property2(coordinate *c, int property, double *result);
void geometry_label2(coordinate *c, double *result);
void geometry_center2(coordinate *c, double *result);
void geometry_centroid2(coordinate *c, double *result);
void geometry_envelope2(coordinate *c, double *result);



#ifdef __cplusplus
}
#endif

#endif // ALGORITHM_H