#include "../coordinate.h"
#include "../math-private.h"
#include "../mathse.h"

static double __alg_arc_length(const double *coord, int length) {
  assert(length > 0);
  double x1;
  double y1;
  double x2;
  double y2;
  double x3;
  double y3;

  double res = 0.0;

  for (int i = 0; i < (length / 2 - 1) / 2; ++i) {
    x1 = coord[0];
    y1 = coord[1];
    x2 = coord[2];
    y2 = coord[3];
    x3 = coord[4];
    y3 = coord[5];

    res += math_circle_length(x1, y1, x2, y2, x3, y3);

    coord += 4;
  }
  return res;
}

static double __alg_path_length(const double *coord, int length) {
  assert(coord && length % 2 == 0);
  double res = 0.0;
  double x1 = coord[0];
  double y1 = coord[1];
  double x2;
  double y2;
  for (int i = 1; i < length / 2; ++i) {
    x2 = coord[i * 2];
    y2 = coord[i * 2 + 1];
    double dx = x2 - x1;
    double dy = y2 - y1;
    res += sqrt(dx * dx + dy * dy);
    x1 = x2;
    y1 = y2;
  }
  return res;
}

static double __alg_bezier_length(const double *coord, int length) {
  assert(length > 0);
  double x1;
  double y1;
  double x2;
  double y2;
  double x3;
  double y3;
  double x4;
  double y4;

  double res = 0.0;

  for (int i = 0; i < (length / 2 - 1) / 3; ++i) {
    x1 = coord[0];
    y1 = coord[1];
    x2 = coord[2];
    y2 = coord[3];
    x3 = coord[4];
    y3 = coord[5];
    x4 = coord[6];
    y4 = coord[7];

    res += math_bezier_length(x1, y1, x2, y2, x3, y3, x4, y4);

    coord += 6;
  }
  return res;
}

static double __alg_envelope_length(const double *coord) {
  assert(coord);
  double x_min = coord[0];
  double y_min = coord[1];
  double x_max = coord[2];
  double y_max = coord[3];

  return 2.0 * (x_max - x_min) + 2.0 * (y_max - y_min);
}

static double __alg_circle_length(const double *coord) {
  double radius = 0.0;
  double cx = 0.0;
  double cy = 0.0;

  math_circle_center_radius(coord[0], coord[1], coord[2], coord[3], coord[4],
                            coord[5], &radius, &cx, &cy);
  return M_PI * radius * radius;
}

/* Calculate geometry length */
double alg_length(coordinate_blob_t *c) {
  assert(c);

  double res = 0.0;
  int offset = 0;
  int len = 0;
  int c_num = c->coord_num;

  interpret_tuple_t *itts = (interpret_tuple_t *)(c->interpret);
  for (int i = 0; i < c->interpret_num; ++i) {
    interpret_tuple_t __itt_cur = itts[i];
    if (i + 1 == c->interpret_num) {
      len = c_num - offset;
    } else {
      interpret_tuple_t _itt_next = itts[i + 1];
      len = _itt_next.Offset - offset;
    }
    if (__itt_cur.Type == INTERPRET_TYPE_POINT)
      continue;

    switch (__itt_cur.Pre) {
    case INTERPRET_PRE_LINE_PATH:
      res += __alg_arc_length(c->coord + offset, len);
      break;

    default:
      break;
    }
  }
  return 0.0;
}