#include "mathse.h"
#include <cairo/cairo.h>

struct se_graphics_context {
  cairo_t *context;
  int width;
  int height;
  graphics_display_transform_t* dt;
};