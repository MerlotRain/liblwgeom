#include "mathSE.h"

double g_tolerance = 0.00001;

double tolerance(double tol) {
  double tmp = g_tolerance;
  g_tolerance = tol;
  return tmp;
}

double geometry_prop_value(const coordinate_blob_t *c, int mode) { return 0.0; }

coordinate_blob_t *geometry_prop_geo(const coordinate_blob_t *c, int mode) {
  return NULL;
}

int geometry_prop_geo2(const coordinate_blob_t *c, int mode, double **result) {
  return 0;
}

int geometry_modify(coordinate_blob_t *c, int mode, void *para) { return 0; }

coordinate_blob_t *geometry_combine(const coordinate_blob_t *a,
                                    const coordinate_blob_t *b, int mode) {
  return NULL;
}

int geometry_spr_query_is(const coordinate_blob_t *a,
                          const coordinate_blob_t *b, int mode) {
  return 0;
}

int geometry_spr_query(const coordinate_blob_t *a, const coordinate_blob_t *b,
                       int *matrix) {
  return 0;
}

int geometry_check_1(const coordinate_blob_t *c, int mode) { return 0; }

int geometry_check_2(const i4_t *r, const int r_n, i4_t **w, int *w_n,
                     int mode) {
  return 0;
}
