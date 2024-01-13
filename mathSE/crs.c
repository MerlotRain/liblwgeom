#include "mathSE.h"
#include <proj.h>

crs_transformation_t *create_trans(int epsg_from, int epsg_to) { return NULL; }

crs_transformation_t *create_wellknown_trans(int wellknown) { return NULL; }

void destroy_trans(crs_transformation_t *trans) {}

int trans_coord(const crs_transformation_t *trans, int direction, double *c) {
  return 0;
}

int trans_coord_array(const crs_transformation_t *trans, int direction,
                      double *c, int c_n) {
  return 0;
}
