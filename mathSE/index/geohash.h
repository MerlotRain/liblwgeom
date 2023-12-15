#ifndef __GEOHASH_H__
#define __GEOHASH_H__

#include <stdbool.h>
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
} geohash_bits;

typedef struct {
  geohash_bits north;
  geohash_bits east;
  geohash_bits west;
  geohash_bits south;
  geohash_bits south_west;
  geohash_bits south_east;
  geohash_bits north_west;
  geohash_bits north_east;
} geohash_neighbors;

typedef struct {
  double max;
  double min;
} geohash_range;

typedef struct {
  geohash_bits hash;
  geohash_range longitude;
  geohash_range latitude;
} geohash_area;

void get_geohash_range(geohash_range *long_range, geohash_range *lat_range);
geohash_bits *geohash_encode(const geohash_range *long_range,
                             const geohash_range *lat_range, double longitude,
                             double latitude, uint8_t step);
geohash_bits *geohash_encode_type(double longitude, double latitude,
                                  uint8_t step);
geohash_bits *geohash_encode_WGS84(double longitude, double latitude,
                                   uint8_t step);
geohash_area *geohash_decode(const geohash_range long_range,
                             const geohash_range lat_range,
                             const geohash_bits hash);
geohash_area *geohash_decode_type(const geohash_range long_range,
                                  const geohash_range lat_range,
                                  const geohash_bits hash);
geohash_area *geohash_decode_WGS84(const geohash_range long_range,
                                   const geohash_range lat_range,
                                   const geohash_bits hash);

bool geohash_decode_area_longlat(const geohash_area *area, double *xy);
bool geohash_decode_longlat_type(const geohash_area *area, double *xy);
bool geohash_decode_longlat_WGS84(const geohash_area *area, double *xy);

geohash_neighbors *geohash_query_neighbors(const geohash_bits *hash);

#ifdef __cplusplus
}
#endif

#endif // __GEOHASH_H__
