#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The geometry memory model is a simplified SDO_Geometry model. */
typedef struct se_coordinate_blob {
  uint8_t geo_type;
  uint32_t interpret_num;
  uint32_t coord_num;
  uint8_t coord_dim;
  uint8_t ref;
  int *interpret;
  double *coord;
} coordinate_blob_t;

typedef struct se_interpret_tuple {
  int Offset; // double offset, begin 1
  int Type;   // element type
  int Pre;    // interpretation
} interpret_tuple_t;

#ifdef __cplusplus
}
#endif

#endif //