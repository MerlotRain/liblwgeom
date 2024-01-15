#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INTERPRET_TYPE_POINT 1
#define INTERPRET_TYPE_LINE 2
#define INTERPRET_TYPE_POLYGON_EXTERIOR 1003
#define INTERPRET_TYPE_POLYGON_INTERIOR 2003
#define INTERPRET_TYPE_COMPOUND_LINE 4
#define INTERPRET_TYPE_COMPOUND_POLYGON_EXTERIOR 1005
#define INTERPRET_TYPE_COMPOUND_POLYGON_INTERIOR 2005

#define INTERPRET_PRE_SINGLE_POINT 1
#define INTERPRET_PRE_LINE_PATH 1
#define INTERPRET_PRE_LINE_ARC 2
#define INTERPRET_PRE_LINE_B_SHAPE 3
#define INTERPRET_PRE_POLYGON_PATH 1
#define INTERPRET_PRE_POLYGON_ARC 2
#define INTERPRET_PRE_POLYGON_RECTANGLE 3
#define INTERPRET_PRE_POLYGON_CIRCLE 4

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

#define INT_TUP_POINT                                                          \
  interpret_tuple_t __itt__ = {                                                \
      .Offset = 1, .Type = INTERPRET_TYPE_POINT, .Pre = 1};

#ifdef __cplusplus
}
#endif

#endif //
