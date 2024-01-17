
#include "coordinate.h"
#include "mathse.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <sys/_types/_null.h>

int compare_raw_point(const raw_point_t a, const raw_point_t b) {
  return a.x == b.x && a.y == b.y;
}

raw_point_t mid_point(const raw_point_t a, const raw_point_t b) {
  raw_point_t pt = {.x = (a.x + b.x) / 2.0, .y = (a.y + b.y) / 2.0};
  return pt;
}

coordinate_blob_t *create_coordinate(const uint8_t geo, const uint32_t i_n,
                                     const int *i_p, const uint32_t c_n,
                                     const double *c_p, const int flags) {
  assert(i_p);
  assert(c_p);
  assert(c_n % 2);
  coordinate_blob_t *coord =
      (coordinate_blob_t *)malloc(sizeof(coordinate_blob_t));
  if (!coord)
    return NULL;

  coord->geo_type = geo;
  coord->coord_num = c_n;
  coord->interpret_num = i_n;
  coord->interpret = (int *)malloc(3 * i_n * sizeof(int));
  if (!coord->interpret) {
    free(coord);
    return NULL;
  }
  memcpy(coord->interpret, i_p, 3 * i_n * sizeof(int));

  coord->ref = flags;
  if (flags == 0) {
    coord->coord = (double *)malloc(c_n * sizeof(double));
    if (!coord->coord) {
      free(coord->interpret);
      free(coord);
      return NULL;
    }
    memcpy(coord->coord, c_p, c_n * sizeof(double));
  } else {
    coord->coord = (double *)c_p;
  }
  return coord;
}

void coordinate_info(const coordinate_blob_t *c, uint32_t *i_n, int **i_p,
                     uint32_t *c_n, double **c_p) {}

void coordinate_destroy(coordinate_blob_t *c) {}

coordinate_blob_t *create_point(const double *p) {
  coordinate_blob_t *coord =
      (coordinate_blob_t *)malloc(sizeof(coordinate_blob_t));
  if (!coord)
    return NULL;

  coord->geo_type = GEOMETRY_TYPE_POINT;
  coord->interpret_num = 1;
  coord->coord_num = 2;
  coord->coord = NULL;
  coord->interpret = NULL;

  coord->interpret = (int *)malloc(3 * sizeof(int));
  if (!coord->interpret) {
    free(coord);
    return NULL;
  }

  interpret_tuple_t __itt__ = INIT_INTERPRET_POINT;
  memcpy(coord->interpret, &__itt__, sizeof(struct se_interpret_tuple));
  coord->interpret[0] = 1;
  coord->interpret[1] = INTERPRET_TYPE_POINT;
  coord->interpret[2] = 1;

  coord->coord = (double *)malloc(2 * sizeof(double));
  if (!coord->coord) {
    free(coord->interpret);
    free(coord);
    coord = NULL;
    return NULL;
  }
  memcpy(coord->coord, p, sizeof(double) * 2);
  return coord;
}

static coordinate_blob_t *create_line_2_point(double x1, double y1, double x2,
                                              double y2) {
  coordinate_blob_t *coord =
      (coordinate_blob_t *)malloc(sizeof(coordinate_blob_t));
  if (!coord)
    return NULL;

  coord->geo_type = GEOMETRY_TYPE_LINE;
  coord->interpret_num = 1;
  coord->coord_num = 3;
  coord->ref = false;
  coord->interpret = (int *)malloc(sizeof(interpret_tuple_t));
  if (!coord->interpret) {
    free(coord);
    coord = NULL;
    return NULL;
  }
  coord->interpret[0] = 1;
  coord->interpret[1] = INTERPRET_TYPE_LINE;
  coord->interpret[2] = INTERPRET_PRE_LINE_PATH;

  coord->coord = (double *)malloc(4 * sizeof(double));
  if (!coord->coord) {
    free(coord->interpret);
    coord->interpret = NULL;
    free(coord);
    coord = NULL;
    return NULL;
  }

  coord->coord[0] = x1;
  coord->coord[1] = y1;
  coord->coord[2] = x2;
  coord->coord[3] = y2;
  return coord;
}

coordinate_blob_t *create_line(const double *p) {
  return create_line_2_point(p[0], p[1], p[2], p[3]);
}

coordinate_blob_t *create_line2(const double *p, const double angle,
                                const double length) {

  double x_end = p[0] + cos(angle) * length;
  double y_end = p[1] + sin(angle) * length;
  return create_line_2_point(p[0], p[1], x_end, y_end);
}

coordinate_blob_t *create_arc(const double *p) { return NULL; }

coordinate_blob_t *create_arc2(const double *c, const double radius,
                               const double angle1, const double angle2) {
  return NULL;
}

coordinate_blob_t *create_arc3(const double *p, const double chord) {
  return NULL;
}

coordinate_blob_t *create_path(const double *p, const int num) { return NULL; }

coordinate_blob_t *create_envelope(const double *p) { return NULL; }

int geometry_type(const coordinate_blob_t *c) { return 0; }

int *coordinate_interpret(const coordinate_blob_t *c) { return NULL; }

int coordinate_interpret_n(const coordinate_blob_t *c) { return 0; }

double *coordinate_coord(const coordinate_blob_t *c) { return NULL; }

unsigned int coordinate_point_n(const coordinate_blob_t *c) { return 0; }

int coordinate_dim_g(const coordinate_blob_t *c) { return 0; }

int coordinate_sub_n(const coordinate_blob_t *c) { return 0; }

coordinate_blob_t *coordinate_sub_at(coordinate_blob_t *c, int index) {
  return NULL;
}
