#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct coordinate_blob {
  uint8_t type;
  uint32_t interpret_num;
  uint32_t coord_num;
  uint8_t coord_dim;
  uint8_t ref;
  int *interpret;
  double *coord;
};

struct interpret_tuple {
  int Offset; // double offset, begin 1
  int Type;   // element type
  int Pre;    // interpretation
};

struct raw_point {
  double x, y;
};

struct raw_point3d {
  double x, y, z;
};

int compare_raw_point(struct raw_point *a, struct raw_point *b);
int compare_raw_point3d(struct raw_point3d *a, struct raw_point3d *b);

#ifdef __cplusplus
}
#endif

#endif // __COORDINATE_H__