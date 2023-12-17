#include "../algorithm.h"
#include <stdio.h>

extern double g_tolerance;

double value_area(coordinate *c) {
  if (c == NULL) {
    perror("value_area: invalid coordinate");
    return 0.0;
  }
}