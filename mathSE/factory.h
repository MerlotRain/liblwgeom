#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <stdint.h>

typedef struct coordinate_blob coordinate;

#ifdef __cplusplus
extern "C" {
#endif

coordinate *create_coordinate(uint32_t i_n, const int *i_p, uint32_t c_n,
                              uint32_t c_dim, const double *c_p, int flags);
void coordinate_info(coordinate *c, uint32_t *i_n, int **i_p, uint32_t *c_n,
                     uint32_t *c_dim, double **c_p);

void coordinate_destroy(coordinate *c);

/* create point */
coordinate *create_point(double *p);
/* create two point line */
coordinate *create_line(double *p);
/* create line by one point and angle with length */
coordinate *create_line2(double *p, double angle, double length);
/* create arc line by three point */
coordinate *create_arc(double *p);
/* create arc by circle and sweep angle */
coordinate *create_arc2(double *c, double radius, double angle1, double angle2);
/* create arc by begin and end point, chord */
coordinate *create_arc3(double *p, double chord);
/* create single path */
coordinate *create_path(double *p, int num);
/* create envelope */
coordinate *create_envelope(double *p);

#ifdef __cplusplus
}
#endif

#endif // __FACTORY_H__
