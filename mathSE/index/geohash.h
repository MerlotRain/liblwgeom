#ifndef MATHSE_GEOHASH_H
#define MATHSE_GEOHASH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __geo_point geo_point_t;

#define GEOHASH_DIRECTION_NORTH 0
#define GEOHASH_DIRECTION_EAST 1
#define GEOHASH_DIRECTION_WEST 2
#define GEOHASH_DIRECTION_SOUTH 3
#define GEOHASH_DIRECTION_SOUTH_WEST 4
#define GEOHASH_DIRECTION_SOUTH_EAST 5
#define GEOHASH_DIRECTION_NORTH_WEST 6
#define GEOHASH_DIRECTION_NORTH_EAST 7

typedef struct {
  uint64_t bits;
  uint8_t step;
} geo_hash_bits;

typedef struct {
  geo_hash_bits north;
  geo_hash_bits east;
  geo_hash_bits west;
  geo_hash_bits south;
  geo_hash_bits south_west;
  geo_hash_bits south_east;
  geo_hash_bits north_west;
  geo_hash_bits north_east;
} geo_hash_neighbors;

typedef struct {
  double max;
  double min;
} geo_hash_range;

typedef struct __geo_hash_shape geo_hash_shape;

void get_get_hash_range(geo_hash_range *long_range, geo_hash_range *lat_range);

void create_geo_point_buckets();

#ifdef __cplusplus
}
#endif

#endif // MATHSE_GEOHASH_H
