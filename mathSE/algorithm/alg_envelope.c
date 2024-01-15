#include "../mathse.h"
#include <assert.h>
#include <float.h>

void alg_calc_envelope(const double *coord, int c_n, double *env) {
  assert(coord);
  double x_min = DBL_MAX;
  double y_min = DBL_MAX;
  double x_max = -DBL_MAX;
  double y_max = -DBL_MAX;

  for (int i = 0; i < c_n; ++i) {
    x_min = MIN(x_min, coord[i * 2]);
    x_max = MAX(x_max, coord[i * 2]);
    y_min = MIN(x_min, coord[i * 2 + 1]);
    y_max = MAX(x_max, coord[i * 2 + 1]);
  }
  env[0] = x_min;
  env[1] = y_min;
  env[2] = x_max;
  env[3] = y_max;
}