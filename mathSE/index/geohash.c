#include "geohash.h"

#define GEO_LAT_MIN -85.05112878
#define GEO_LAT_MAX 85.05112878
#define GEO_LONG_MIN -180
#define GEO_LONG_MAX 180

struct geo_point {
  double longitude;
  double latitude;
  double code;
  char *name;
};

struct geo_array {
  struct geo_point *array;
  size_t buckets;
  size_t used;
};

struct geo_array *g_geo_array = NULL;

