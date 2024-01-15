#include "coordinate.h"
#include "mathse.h"
#include <assert.h>

extern double alg_polygon_area(const coordinate_blob_t *c);
extern void alg_calc_envelope(const double *coord, int c_n, double *env);

static double geo_prop_value_area(const coordinate_blob_t *c) {
  assert(c);
  assert(c->interpret);
  assert(c->coord);

  interpret_tuple_t *inter = (interpret_tuple_t *)(c->interpret);
  if (inter == NULL)
    return 0.0;
  switch (inter[0].Type) {
  case INTERPRET_TYPE_POINT:
  case INTERPRET_TYPE_LINE:
  case INTERPRET_TYPE_COMPOUND_LINE:
    return 0.0;
  }

  return 0.0;
}

static double geo_prop_value_height(const coordinate_blob_t *c) {
  raw_box_t box;
  alg_calc_envelope(c->coord, c->coord_num, (double *)&box);
  return box.upper_right.y - box.lower_left.y;
}

static double geo_prop_value_width(const coordinate_blob_t *c) {
  raw_box_t box;
  alg_calc_envelope(c->coord, c->coord_num, (double *)&box);
  return box.upper_right.x - box.lower_left.x;
}

static double geo_prop_value_length(const coordinate_blob_t *c) { return 0.0; }

double g_tolerance = 0.000001;

double tolerance(double tol) {
  double tmp = g_tolerance;
  g_tolerance = tol;
  return tmp;
}

double geometry_prop_value(const coordinate_blob_t *c, int mode) {
  assert(c);

  switch (mode) {
  case GEOMETRY_PROP_VALUE_AREA:
    return geo_prop_value_area(c);
  case GEOMETRY_PROP_VALUE_HEIGHT:
    return geo_prop_value_height(c);
  case GEOMETRY_PROP_VALUE_WIDTH:
    return geo_prop_value_width(c);
  case GEOMETRY_PROP_VALUE_LENGTH:
    return geo_prop_value_length(c);
  default:
    fprintf(stderr, "Unknown mode: %d\n", mode);
  }
  return 0.0;
}

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
