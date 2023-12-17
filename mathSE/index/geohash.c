#include "geohash.h"
#include <stddef.h>

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

void init_geo_point_buckets() {
  if (g_geo_array == NULL) {
    g_geo_array = malloc(sizeof(struct geo_array));
    if (g_geo_array == NULL)
      return;
  }
}
/* http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN */
static inline uint64_t interleave64(uint32_t xlo, uint32_t ylo) {
  static const uint64_t B[] = {0x5555555555555555ULL, 0x3333333333333333ULL,
                               0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
                               0x0000FFFF0000FFFFULL};
  static const int S[] = {1, 2, 4, 8, 16};
  uint64_t x = xlo;
  uint64_t y = ylo;

  x = (x | (x << S[4])) & B[4];
  y = (y | (y << S[4])) & B[4];

  x = (x | (x << S[3])) & B[3];
  y = (y | (y << S[3])) & B[3];

  x = (x | (x << S[2])) & B[2];
  y = (y | (y << S[2])) & B[2];

  x = (x | (x << S[1])) & B[0];
  y = (y | (y << S[1])) & B[0];

  x = (x | (x << S[0])) & B[0];
  y = (y | (y << S[0])) & B[0];

  return x | (y << 1);
}

static inline uint64_t deinterleave64(uint64_t interleaved) {
  static const uint64_t B[] = {0x5555555555555555ULL, 0x3333333333333333ULL,
                               0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
                               0x0000FFFF0000FFFFULL};
  static const int S[] = {1, 2, 4, 8, 16};
  uint64_t x = interleaved;
  uint64_t y = interleaved >> 1;

  x = (x | (x >> S[4])) & B[4];
  y = (y | (y >> S[4])) & B[4];

  x = (x | (x >> S[3])) & B[3];
  y = (y | (y >> S[3])) & B[3];

  x = (x | (x >> S[2])) & B[2];
  y = (y | (y >> S[2])) & B[2];

  x = (x | (x >> S[1])) & B[0];
  y = (y | (y >> S[1])) & B[0];

  x = (x | (x >> S[0])) & B[0];
  y = (y | (y >> S[0])) & B[0];

  return x | (y << 32);
}

void get_geohash_range(geohash_range *long_range, geohash_range *lat_range) {
  long_range->max = GEO_LONG_MAX;
  long_range->min = GEO_LONG_MIN;
  lat_range->max = GEO_LAT_MAX;
  lat_range->min = GEO_LAT_MIN;
}

geohash_bits *geohash_encode(const geohash_range *long_range,
                             const geohash_range *lat_range, double longitude,
                             double latitude, uint8_t step) {
  return NULL;
}

geohash_bits *geohash_encode_type(double longitude, double latitude,
                                  uint8_t step) {
  return NULL;
}

geohash_bits *geohash_encode_WGS84(double longitude, double latitude,
                                   uint8_t step) {
  return NULL;
}

geohash_area *geohash_decode(const geohash_range long_range,
                             const geohash_range lat_range,
                             const geohash_bits hash) {
  return NULL;
}

geohash_area *geohash_decode_type(const geohash_range long_range,
                                  const geohash_range lat_range,
                                  const geohash_bits hash) {
  return NULL;
}

geohash_area *geohash_decode_WGS84(const geohash_range long_range,
                                   const geohash_range lat_range,
                                   const geohash_bits hash) {
  return NULL;
}

bool geohash_decode_area_longlat(const geohash_area *area, double *xy) {
  return false;
}

bool geohash_decode_longlat_type(const geohash_area *area, double *xy) {
  return false;
}

bool geohash_decode_longlat_WGS84(const geohash_area *area, double *xy) {
  return false;
}

geohash_neighbors *geohash_query_neighbors(const geohash_bits *hash) {
  return NULL;
}
